#pragma once

#include <fstream>
#include <stdexcept>
#include <string>


inline void WriteSieveVisualizationHtml(int high, const std::string& filename) {
    if (high < 2) {
        throw std::invalid_argument("Sieve visualization: high must be >= 2");
    }

    static const int MAX_VISUAL_HIGH = 300;

    if (high > MAX_VISUAL_HIGH) {
        throw std::invalid_argument("Sieve visualization: high is too large for visualization");
    }

    bool* isPrime = nullptr;
    std::ofstream out(filename);

    if (!out.is_open()) {
        throw std::runtime_error("Cannot open output html file");
    }

    try {
        isPrime = new bool[high + 1];

        for (int i = 0; i <= high; ++i) {
            isPrime[i] = true;
        }

        isPrime[0] = false;
        isPrime[1] = false;

        out << "<!DOCTYPE html>\n";
        out << "<html lang=\"en\">\n";
        out << "<head>\n";
        out << "    <meta charset=\"UTF-8\">\n";
        out << "    <title>Sieve of Eratosthenes Visualization</title>\n";
        out << "    <style>\n";
        out << "        body { font-family: Arial, sans-serif; padding: 20px; }\n";
        out << "        button { padding: 8px 14px; margin-right: 8px; }\n";
        out << "        .cell { fill: white; stroke: #333; stroke-width: 1; }\n";
        out << "        .number { font-size: 14px; text-anchor: middle; dominant-baseline: middle; }\n";
        out << "        .active { fill: #ffe08a; }\n";
        out << "        .prime { fill: #9be7a1; }\n";
        out << "        .crossed { fill: #ddd; }\n";
        out << "        .cross-line { stroke: #c0392b; stroke-width: 3; }\n";
        out << "    </style>\n";
        out << "</head>\n";
        out << "<body>\n";
        out << "    <h2>Sieve of Eratosthenes, n = " << high << "</h2>\n";
        out << "    <p id=\"info\">Press Start to begin.</p>\n";
        out << "    <button onclick=\"startAnimation()\">Start</button>\n";
        out << "    <button onclick=\"resetAnimation()\">Reset</button>\n";
        out << "    <br><br>\n";
        out << "    <svg id=\"sieve\"></svg>\n";

        out << "    <script>\n";
        out << "        const high = " << high << ";\n";
        out << "        const cols = 10;\n";
        out << "        const cellW = 70;\n";
        out << "        const cellH = 50;\n";
        out << "        const startX = 20;\n";
        out << "        const startY = 20;\n";
        out << "        const svg = document.getElementById('sieve');\n";
        out << "        const info = document.getElementById('info');\n";

        out << "        const count = high - 1;\n";
        out << "        const rows = Math.ceil(count / cols);\n";
        out << "        const svgWidth = startX * 2 + cols * cellW;\n";
        out << "        const svgHeight = startY * 2 + rows * cellH;\n";
        out << "        svg.setAttribute('width', svgWidth);\n";
        out << "        svg.setAttribute('height', svgHeight);\n";
        out << "        svg.setAttribute('viewBox', '0 0 ' + svgWidth + ' ' + svgHeight);\n";

        out << "        const steps = [\n";

        bool firstStep = true;

        for (int p = 2; p <= high / p; ++p) {
            if (isPrime[p]) {
                if (!firstStep) {
                    out << ",\n";
                }

                out << "            { prime: " << p << ", crossed: [";

                bool firstCrossed = true;

                for (int multiple = p * p; multiple <= high; multiple += p) {
                    if (isPrime[multiple]) {
                        isPrime[multiple] = false;

                        if (!firstCrossed) {
                            out << ", ";
                        }

                        out << multiple;
                        firstCrossed = false;
                    }
                }

                out << "] }";
                firstStep = false;
            }
        }

        out << "\n        ];\n";

        out << R"JS(
        function cellId(n) {
            return 'cell-' + n;
        }

        function textId(n) {
            return 'text-' + n;
        }

        function lineId(n) {
            return 'line-' + n;
        }

        function getPosition(n) {
            const index = n - 2;
            const row = Math.floor(index / cols);
            const col = index % cols;

            return {
                x: startX + col * cellW,
                y: startY + row * cellH
            };
        }

        function createSvgElement(name) {
            return document.createElementNS('http://www.w3.org/2000/svg', name);
        }

        function drawGrid() {
            svg.innerHTML = '';

            for (let n = 2; n <= high; ++n) {
                const pos = getPosition(n);

                const rect = createSvgElement('rect');
                rect.setAttribute('id', cellId(n));
                rect.setAttribute('class', 'cell');
                rect.setAttribute('x', pos.x);
                rect.setAttribute('y', pos.y);
                rect.setAttribute('width', 55);
                rect.setAttribute('height', 35);
                rect.setAttribute('rx', 6);

                const text = createSvgElement('text');
                text.setAttribute('id', textId(n));
                text.setAttribute('class', 'number');
                text.setAttribute('x', pos.x + 27.5);
                text.setAttribute('y', pos.y + 18);
                text.textContent = n;

                svg.appendChild(rect);
                svg.appendChild(text);
            }
        }

        function markActive(n) {
            const cell = document.getElementById(cellId(n));
            if (cell) {
                cell.setAttribute('class', 'cell active');
            }
        }

        function markPrime(n) {
            const cell = document.getElementById(cellId(n));
            if (cell) {
                cell.setAttribute('class', 'cell prime');
            }
        }

        function crossNumber(n) {
            const cell = document.getElementById(cellId(n));
            if (cell) {
                cell.setAttribute('class', 'cell crossed');
            }

            if (document.getElementById(lineId(n))) {
                return;
            }

            const pos = getPosition(n);

            const line = createSvgElement('line');
            line.setAttribute('id', lineId(n));
            line.setAttribute('class', 'cross-line');
            line.setAttribute('x1', pos.x + 8);
            line.setAttribute('y1', pos.y + 28);
            line.setAttribute('x2', pos.x + 47);
            line.setAttribute('y2', pos.y + 7);

            svg.appendChild(line);
        }

        function resetAnimation() {
            drawGrid();
            info.textContent = 'Press Start to begin.';
        }

        function startAnimation() {
            resetAnimation();

            let stepIndex = 0;

            function runStep() {
                if (stepIndex >= steps.length) {
                    for (let n = 2; n <= high; ++n) {
                        const cell = document.getElementById(cellId(n));
                        const hasLine = document.getElementById(lineId(n));

                        if (cell && !hasLine) {
                            cell.setAttribute('class', 'cell prime');
                        }
                    }

                    info.textContent = 'Done. Green cells are prime numbers.';
                    return;
                }

                const step = steps[stepIndex];
                info.textContent = 'Prime = ' + step.prime + ', crossing out: [' + step.crossed.join(', ') + ']';

                markActive(step.prime);

                let i = 0;

                function crossNext() {
                    if (i >= step.crossed.length) {
                        markPrime(step.prime);
                        ++stepIndex;
                        setTimeout(runStep, 700);
                        return;
                    }

                    crossNumber(step.crossed[i]);
                    ++i;
                    setTimeout(crossNext, 250);
                }

                crossNext();
            }

            runStep();
        }

        drawGrid();
)JS";

        out << "    </script>\n";
        out << "</body>\n";
        out << "</html>\n";

        delete[] isPrime;
    }
    catch (...) {
        delete[] isPrime;
        throw;
    }
}