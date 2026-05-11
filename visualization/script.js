let data = null;

fetch('data.json')
    .then(res => res.json())
    .then(json => data = json);

function start(type) {
    if (!data) {
        alert("Wait for data!");
        return;
    }

    const sequence = data[type];

    const seekTime = calculateSeekTime(sequence);

    document.getElementById("info").innerHTML =
        `<h3>${type.toUpperCase()} Seek Time: ${seekTime}</h3>`;

    animate(sequence, type);
}

function animate(sequence, type) {
    const canvas = document.getElementById("canvas");
    const ctx = canvas.getContext("2d");

    const scale = canvas.width / 200;
    const y = 200;

    let i = 0;
    let currentX = sequence[0] * scale;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawBaseLine(ctx, canvas, y);

    function moveToNext() {
        if (i >= sequence.length - 1) return;

        let targetX = sequence[i + 1] * scale;

        // Detect jump (C-SCAN)
        let isJump = (type === 'cscan' && sequence[i + 1] < sequence[i]);

        function smoothMove() {
            let dx = targetX - currentX;

            if (Math.abs(dx) < 1) {
                // Snap to target
                currentX = targetX;

                // Draw final point
                drawPoint(ctx, currentX, y, sequence[i + 1]);

                i++;
                setTimeout(moveToNext, 300);
                return;
            }

            currentX += dx * 0.1; // smooth easing

            redraw();

            requestAnimationFrame(smoothMove);
        }

        function redraw() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            drawBaseLine(ctx, canvas, y);

            // Draw previous completed path
            ctx.strokeStyle = (type === 'scan') ? '#00c6ff' : '#ff4d4d';

            ctx.beginPath();

            for (let j = 0; j < i; j++) {
                let x1 = sequence[j] * scale;
                let x2 = sequence[j + 1] * scale;

                if (type === 'cscan' && sequence[j + 1] < sequence[j]) {
                    ctx.setLineDash([6, 6]);
                } else {
                    ctx.setLineDash([]);
                }

                ctx.moveTo(x1, y);
                ctx.lineTo(x2, y);
            }

            ctx.stroke();
            ctx.setLineDash([]);

            // Draw moving line
            ctx.beginPath();
            ctx.moveTo(sequence[i] * scale, y);
            ctx.lineTo(currentX, y);
            ctx.stroke();
            ctx.shadowBlur = 10;
ctx.shadowColor = ctx.strokeStyle;

            // Draw all points
            for (let k = 0; k <= i + 1; k++) {
                let x = sequence[k] * scale;
                drawPoint(ctx, x, y, sequence[k]);
            }

            // Draw moving head
            ctx.beginPath();
            ctx.arc(currentX, y, 8, 0, Math.PI * 2);
            ctx.fillStyle = "#00ffcc";
            ctx.fill();
        }

        smoothMove();
    }

    moveToNext();
}
    function step() {
        if (i >= sequence.length - 1) return;

        let x1 = sequence[i] * scale;
        let x2 = sequence[i + 1] * scale;

        // Color
        ctx.strokeStyle = (type === 'scan') ? 'blue' : 'red';
        ctx.lineWidth = 2;

        // Detect C-SCAN jump
        if (type === 'cscan' && sequence[i + 1] < sequence[i]) {
            ctx.setLineDash([6, 6]);
        } else {
            ctx.setLineDash([]);
        }

        // Draw line
        ctx.beginPath();
        ctx.moveTo(x1, y);
        ctx.lineTo(x2, y);
        ctx.stroke();

        // Draw points
        drawPoint(ctx, x1, y, sequence[i]);
        drawPoint(ctx, x2, y, sequence[i + 1]);

        // Draw current head (circle)
        ctx.beginPath();
        ctx.arc(x2, y, 6, 0, Math.PI * 2);
        ctx.fillStyle = "black";
        ctx.fill();

        i++;
        setTimeout(step, 600); // animation speed
    }

    step();
function calculateSeekTime(sequence) {
    let total = 0;

    for (let i = 0; i < sequence.length - 1; i++) {
        total += Math.abs(sequence[i + 1] - sequence[i]);
    }

    return total;
}

function drawBaseLine(ctx, canvas, y) {
    ctx.strokeStyle = "#000";
    ctx.lineWidth = 1;

    ctx.beginPath();
    ctx.moveTo(0, y);
    ctx.lineTo(canvas.width, y);
    ctx.stroke();

    // Axis labels
    ctx.fillText("0", 0, y + 20);
    ctx.fillText("200", canvas.width - 30, y + 20);
}

function drawPoint(ctx, x, y, label) {
    ctx.fillStyle = "black";
    ctx.fillRect(x - 3, y - 3, 6, 6);

    ctx.fillText(label, x - 10, y - 10);
}