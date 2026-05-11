// ============================================================
//  script.js  —  Disk Scheduling Visualization
//
//  TEAM NOTES:
//  This file calls the C++ API server running on port 8080.
//  Make sure server.exe is running before opening this page.
//
//  Flow:
//    1. User fills in inputs and clicks a button
//    2. run() collects inputs and sends POST request to API
//    3. API returns JSON with sequence, seek_time, steps[]
//    4. drawVisualization() draws the result on canvas
//    5. animateHead() animates the head movement step by step
// ============================================================

const API = "http://localhost:8080";

// ============================================================
//  run(type)
//  Called by the buttons in index.html
//  type = 'scan' | 'cscan' | 'compare'
// ============================================================
async function run(type) {
    // Clear previous results and errors
    document.getElementById("error").innerText = "";
    document.getElementById("scan-section").style.display  = "none";
    document.getElementById("cscan-section").style.display = "none";
    document.getElementById("compare-result").innerHTML    = "";

    // Collect inputs
    const disk_size = parseInt(document.getElementById("disk_size").value);
    const head      = parseInt(document.getElementById("head").value);
    const direction = document.getElementById("direction").value;
    const rawReqs   = document.getElementById("requests").value;

    // Parse requests string "98,183,41" → [98, 183, 41]
    const requests  = rawReqs.split(",").map(s => parseInt(s.trim())).filter(n => !isNaN(n));

    // Basic frontend validation before even calling the API
    if (isNaN(disk_size) || isNaN(head) || requests.length === 0) {
        showError("Please fill in all fields correctly.");
        return;
    }

    // Build the JSON body
    const body = { disk_size, head, direction, requests };

    try {
        if (type === "compare") {
            // Call /compare endpoint
            const data = await callAPI("/compare", body);

            // Show both canvases
            showSection("scan",  data.scan,  disk_size, head);
            showSection("cscan", data.cscan, disk_size, head);

            // Show winner
            document.getElementById("compare-result").innerHTML =
                `<h3 style="text-align:center">
                    Winner: <span style="color:#00ffcc">${data.winner}</span>
                    &nbsp;|&nbsp; SCAN: ${data.scan.seek_time} 
                    &nbsp;vs&nbsp; C-SCAN: ${data.cscan.seek_time}
                </h3>`;
        } else {
            // Call /scan or /cscan endpoint
            const data = await callAPI("/" + type, body);
            showSection(type, data, disk_size, head);
        }
    } catch (err) {
        showError(err.message);
    }
}


// ============================================================
//  callAPI(endpoint, body)
//  Sends a POST request to the C++ API server
//  Returns parsed JSON response
//  Throws an error if the request fails or API returns error
// ============================================================
async function callAPI(endpoint, body) {
    const response = await fetch(API + endpoint, {
        method:  "POST",
        headers: { "Content-Type": "application/json" },
        body:    JSON.stringify(body)
    });

    const data = await response.json();

    // If API returned an error (400), throw it so run() can catch it
    if (!response.ok) {
        throw new Error(data.error || "API error");
    }

    return data;
}


// ============================================================
//  showSection(type, data, disk_size, head)
//  Shows the canvas section for scan or cscan
//  Sets the title and starts the animation
// ============================================================
function showSection(type, data, disk_size, head) {
    const section = document.getElementById(type + "-section");
    const title   = document.getElementById(type + "-title");
    const canvas  = document.getElementById(type + "-canvas");

    section.style.display = "block";
    title.innerText = `${data.algorithm || type.toUpperCase()} — Seek Time: ${data.seek_time}`;

    drawVisualization(canvas, data.steps, data.sequence, disk_size, head, type);
}


// ============================================================
//  drawVisualization(canvas, steps, sequence, disk_size, head, type)
//  Draws the disk line, all track points, then animates
// ============================================================
function drawVisualization(canvas, steps, sequence, disk_size, head, type) {
    const ctx   = canvas.getContext("2d");
    const W     = canvas.width;
    const H     = canvas.height;
    const y     = H / 2;                    // vertical center line
    const scale = (W - 60) / disk_size;     // pixels per track unit, with padding
    const pad   = 30;                        // left/right padding

    // Convert track number to canvas X coordinate
    const toX = track => pad + track * scale;

    // Clear canvas
    ctx.clearRect(0, 0, W, H);

    // Draw the horizontal disk line
    ctx.strokeStyle = "#555";
    ctx.lineWidth   = 2;
    ctx.setLineDash([]);
    ctx.beginPath();
    ctx.moveTo(pad, y);
    ctx.lineTo(W - pad, y);
    ctx.stroke();

    // Draw axis labels (0 and disk_size)
    ctx.fillStyle  = "#aaa";
    ctx.font       = "12px monospace";
    ctx.fillText("0", pad - 5, y + 25);
    ctx.fillText(disk_size, W - pad - 15, y + 25);

    // Draw initial head position marker
    ctx.fillStyle = "#ffcc00";
    ctx.font      = "12px monospace";
    ctx.fillText("HEAD", toX(head) - 15, y - 40);
    ctx.beginPath();
    ctx.arc(toX(head), y, 6, 0, Math.PI * 2);
    ctx.fill();

    // Draw all request points on the line
    const color = type === "scan" ? "#00c6ff" : "#ff4d4d";
    sequence.forEach(track => {
        ctx.fillStyle = color;
        ctx.beginPath();
        ctx.arc(toX(track), y, 4, 0, Math.PI * 2);
        ctx.fill();

        ctx.fillStyle = "#ccc";
        ctx.font      = "11px monospace";
        ctx.fillText(track, toX(track) - 8, y + 20);
    });

    // Start animation
    animateHead(ctx, steps, toX, y, type, head);
}


// ============================================================
//  animateHead(ctx, steps, toX, y, type, head)
//  Animates the head moving through each step one by one
//  Uses the steps[] array from the API response
//  Each step: { from, to, distance }
// ============================================================
function animateHead(ctx, steps, toX, y, type, startHead) {
    const color     = type === "scan" ? "#00c6ff" : "#ff4d4d";
    const jumpColor = "#888";   // dashed grey line for C-SCAN jump
    let stepIndex   = 0;
    const drawn     = [];       // keeps track of lines already drawn

    function drawStep() {
        if (stepIndex >= steps.length) return;

        const step   = steps[stepIndex];
        const x1     = toX(step.from);
        const x2     = toX(step.to);

        // Detect C-SCAN jump: head went from high track back to 0
        const isJump = (type === "cscan" && step.to < step.from);

        // Draw the movement line
        ctx.beginPath();
        ctx.moveTo(x1, y);
        ctx.lineTo(x2, y);
        ctx.strokeStyle = isJump ? jumpColor : color;
        ctx.lineWidth   = 2;
        ctx.setLineDash(isJump ? [6, 4] : []);   // dashed for jump
        ctx.stroke();
        ctx.setLineDash([]);

        // Draw the head circle at destination
        ctx.beginPath();
        ctx.arc(x2, y, 7, 0, Math.PI * 2);
        ctx.fillStyle = "#00ffcc";
        ctx.fill();

        // Label the step distance above the midpoint
        const midX = (x1 + x2) / 2;
        ctx.fillStyle = "#fff";
        ctx.font      = "10px monospace";
        ctx.fillText(step.distance, midX - 8, y - 15);

        drawn.push(step);
        stepIndex++;

        // Wait 600ms before drawing next step
        setTimeout(drawStep, 600);
    }

    // Start after a short delay
    setTimeout(drawStep, 300);
}


// ============================================================
//  showError(msg)
//  Displays an error message on the page
// ============================================================
function showError(msg) {
    document.getElementById("error").innerText = "Error: " + msg;
}
