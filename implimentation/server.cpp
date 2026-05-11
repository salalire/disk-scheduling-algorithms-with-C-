// ============================================================
//  server.cpp  —  Disk Scheduling API Server
//  Framework : Crow (single-header C++ HTTP library)
//  Port      : 8080
//
//  TEAM NOTES:
//  This file is the bridge between the C++ algorithms and
//  the frontend. It does NOT rewrite any algorithm logic.
//  It simply:
//    1. Receives JSON from the frontend via HTTP POST
//    2. Calls the existing scan/cscan/comparison functions
//    3. Returns the result as JSON back to the frontend
//
//  Endpoints:
//    POST /scan     — run SCAN algorithm
//    POST /cscan    — run C-SCAN algorithm
//    POST /compare  — run both and compare
//
//  Expected JSON input for all endpoints:
//  {
//    "disk_size"  : 200,
//    "head"       : 50,
//    "direction"  : "right",   <-- only used by /scan and /compare
//    "requests"   : [98, 183, 41, 122, 14, 124, 65, 67]
//  }
// ============================================================

#define CROW_ENABLE_SSL 0
#include "crow_all.h"   // Crow framework (single header)
#include "scan.h"       // scan() and calculateSeekTime()
#include "cscan.h"      // cscan()
#include "comparison.h" // compareAlgorithms() — not used directly here
                        // but kept for reference

#include <vector>
#include <string>
#include <cmath>

using namespace std;


// ============================================================
//  HELPER: buildSteps
//
//  TEAM NOTES:
//  This helper converts a seek sequence into a step-by-step
//  movement list. Each step shows:
//    - from  : where the head currently is
//    - to    : where it moves next
//    - distance : how far it moved (abs difference)
//
//  This is what the frontend will use to animate the head.
// ============================================================
crow::json::wvalue buildSteps(const vector<int>& sequence, int head) {
    crow::json::wvalue steps;
    int current = head;
    int idx = 0;

    for (int pos : sequence) {
        crow::json::wvalue step;
        step["from"]     = current;
        step["to"]       = pos;
        step["distance"] = abs(pos - current);
        steps[idx++]     = move(step);
        current = pos;
    }

    return steps;
}


// ============================================================
//  HELPER: parseRequest
//
//  TEAM NOTES:
//  Reads the JSON body and extracts:
//    disk_size, head, direction, requests[]
//
//  Returns false if any required field is missing or invalid.
//  The error message is written into `err` so the endpoint
//  can send it back to the frontend.
// ============================================================
bool parseRequest(
    const crow::json::rvalue& body,
    int& disk_size,
    int& head,
    string& direction,
    vector<int>& requests,
    string& err
) {
    // Check required fields exist
    if (!body.has("disk_size") || !body.has("head") || !body.has("requests")) {
        err = "Missing required fields: disk_size, head, requests";
        return false;
    }

    disk_size = body["disk_size"].i();
    head      = body["head"].i();

    // direction is optional for cscan (cscan always goes right)
    // but required for scan and compare
    if (body.has("direction"))
        direction = body["direction"].s();
    else
        direction = "right"; // safe default

    // Parse requests array
    for (auto& r : body["requests"])
        requests.push_back(r.i());

    // Validate values
    if (disk_size <= 0) {
        err = "disk_size must be greater than 0";
        return false;
    }
    if (head < 0 || head >= disk_size) {
        err = "head position must be between 0 and disk_size - 1";
        return false;
    }
    if (direction != "left" && direction != "right") {
        err = "direction must be 'left' or 'right'";
        return false;
    }
    if (requests.empty()) {
        err = "requests array cannot be empty";
        return false;
    }
    for (int r : requests) {
        if (r < 0 || r >= disk_size) {
            err = "All requests must be between 0 and disk_size - 1";
            return false;
        }
    }

    return true;
}


// ============================================================
//  MAIN — Server Setup
// ============================================================
int main() {
    // ----------------------------------------------------------
    //  CORS Middleware
    //
    //  TEAM NOTES:
    //  CORS = Cross-Origin Resource Sharing.
    //  Without this, the browser will BLOCK the frontend from
    //  calling this API because they run on different ports
    //  (frontend on 5500, API on 8080).
    //
    //  In Crow, middleware is passed as a template parameter
    //  to App<>. CORSHandler is Crow's built-in CORS middleware.
    //  We configure it to allow all origins (*) so the frontend
    //  can call the API from any port during development.
    // ----------------------------------------------------------
    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors
        .global()
        .origin("*")
        .headers("Content-Type")
        .methods("POST"_method, "GET"_method, "OPTIONS"_method);


    // ==========================================================
    //  ENDPOINT: GET /
    //  Health check — lets the frontend verify the server is up
    // ==========================================================
    CROW_ROUTE(app, "/")
    ([]() {
        crow::json::wvalue res;
        res["status"]  = "running";
        res["message"] = "Disk Scheduling API is live";
        res["endpoints"] = {"/scan", "/cscan", "/compare"};
        return crow::response(200, res);
    });


    // ==========================================================
    //  ENDPOINT: POST /scan
    //
    //  TEAM NOTES:
    //  Runs the SCAN (elevator) algorithm.
    //  Direction matters here — head moves in given direction,
    //  hits the end, then reverses.
    //
    //  Response JSON:
    //  {
    //    "algorithm" : "SCAN",
    //    "seek_time" : 208,
    //    "sequence"  : [65, 67, 98, 122, 124, 183, 199, 41, 14],
    //    "steps": [
    //      { "from": 50, "to": 65, "distance": 15 },
    //      ...
    //    ]
    //  }
    // ==========================================================
    CROW_ROUTE(app, "/scan").methods("POST"_method)
    ([](const crow::request& req) {
        // Parse JSON body
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "{\"error\": \"Invalid JSON body\"}");

        int disk_size, head;
        string direction;
        vector<int> requests;
        string err;

        if (!parseRequest(body, disk_size, head, direction, requests, err)) {
            crow::json::wvalue errRes;
            errRes["error"] = err;
            return crow::response(400, errRes);
        }

        // Run SCAN algorithm (from scan.cpp)
        vector<int> seq  = scan(requests, head, disk_size, direction);
        int seek         = calculateSeekTime(seq, head);

        // Build response
        crow::json::wvalue res;
        res["algorithm"] = "SCAN";
        res["seek_time"] = seek;
        res["steps"]     = buildSteps(seq, head);

        for (int i = 0; i < (int)seq.size(); i++)
            res["sequence"][i] = seq[i];

        return crow::response(200, res);
    });


    // ==========================================================
    //  ENDPOINT: POST /cscan
    //
    //  TEAM NOTES:
    //  Runs the C-SCAN (Circular SCAN) algorithm.
    //  Direction is always RIGHT — head goes to the end,
    //  jumps back to 0, and continues. No reversal.
    //  This gives more uniform wait times across all tracks.
    //
    //  Note: direction field is ignored for C-SCAN.
    //
    //  Response JSON: same structure as /scan
    // ==========================================================
    CROW_ROUTE(app, "/cscan").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "{\"error\": \"Invalid JSON body\"}");

        int disk_size, head;
        string direction;
        vector<int> requests;
        string err;

        if (!parseRequest(body, disk_size, head, direction, requests, err)) {
            crow::json::wvalue errRes;
            errRes["error"] = err;
            return crow::response(400, errRes);
        }

        // Run C-SCAN algorithm (from cscan.cpp)
        vector<int> seq  = cscan(requests, head, disk_size);
        int seek         = calculateSeekTime(seq, head);

        // Build response
        crow::json::wvalue res;
        res["algorithm"] = "C-SCAN";
        res["seek_time"] = seek;
        res["steps"]     = buildSteps(seq, head);

        for (int i = 0; i < (int)seq.size(); i++)
            res["sequence"][i] = seq[i];

        return crow::response(200, res);
    });


    // ==========================================================
    //  ENDPOINT: POST /compare
    //
    //  TEAM NOTES:
    //  Runs BOTH algorithms on the same input and returns
    //  results side by side. The frontend uses this to show
    //  the comparison view.
    //
    //  Response JSON:
    //  {
    //    "winner": "SCAN",
    //    "scan": {
    //      "seek_time": 208,
    //      "sequence": [...],
    //      "steps": [...]
    //    },
    //    "cscan": {
    //      "seek_time": 322,
    //      "sequence": [...],
    //      "steps": [...]
    //    }
    //  }
    // ==========================================================
    CROW_ROUTE(app, "/compare").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "{\"error\": \"Invalid JSON body\"}");

        int disk_size, head;
        string direction;
        vector<int> requests;
        string err;

        if (!parseRequest(body, disk_size, head, direction, requests, err)) {
            crow::json::wvalue errRes;
            errRes["error"] = err;
            return crow::response(400, errRes);
        }

        // Run both algorithms
        vector<int> scanSeq  = scan(requests, head, disk_size, direction);
        vector<int> cscanSeq = cscan(requests, head, disk_size);
        int scanSeek         = calculateSeekTime(scanSeq, head);
        int cscanSeek        = calculateSeekTime(cscanSeq, head);

        // Determine winner
        string winner;
        if (scanSeek < cscanSeek)       winner = "SCAN";
        else if (cscanSeek < scanSeek)  winner = "C-SCAN";
        else                            winner = "TIE";

        // Build response
        crow::json::wvalue res;
        res["winner"] = winner;

        res["scan"]["seek_time"] = scanSeek;
        res["scan"]["steps"]     = buildSteps(scanSeq, head);
        for (int i = 0; i < (int)scanSeq.size(); i++)
            res["scan"]["sequence"][i] = scanSeq[i];

        res["cscan"]["seek_time"] = cscanSeek;
        res["cscan"]["steps"]     = buildSteps(cscanSeq, head);
        for (int i = 0; i < (int)cscanSeq.size(); i++)
            res["cscan"]["sequence"][i] = cscanSeq[i];

        return crow::response(200, res);
    });


    // ----------------------------------------------------------
    //  Start the server
    //
    //  TEAM NOTES:
    //  - Runs on port 8080
    //  - multithreaded() allows handling multiple requests at once
    //  - Visit http://localhost:8080 to verify it's running
    // ----------------------------------------------------------
    cout << "====================================\n";
    cout << "  Disk Scheduling API Server\n";
    cout << "  Running on http://localhost:8080\n";
    cout << "====================================\n";

    app.port(8080).multithreaded().run();

    return 0;
}
