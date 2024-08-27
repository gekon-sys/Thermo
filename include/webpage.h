const char MAIN_PAGE[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ThermoPlant</title>
    <!-- Include Font Awesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
    <!-- Include Chart.js -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js@latest"></script>
    <style>
        body {
            background-color: #1A1A1A;
            margin: 0;
            font-family: 'Roboto', sans-serif;
            color: #FFFFFF;
        }
        .header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 20px;
            background: #1A1A1A;
            color: #fff;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        .title {
            font-size: 36px;
            font-weight: bold;
            display: flex;
            align-items: center;
        }
        .title i {
            color: #EC5800; /* Icon color */
            margin-right: 10px;
            font-size: 40px; /* Icon size */
        }
        .datetime {
            font-size: 18px;
        }
        hr {
            border: none;
            border-top: 3px solid #EC5800;
            margin: 0;
        }
        .container {
            display: flex;
            flex-direction: column;
            margin-top: 20px;
            padding: 0 20px;
        }
        .tables-container {
            display: flex;
            justify-content: space-between;
            width: 100%;
            max-width: 1200px;
            margin-bottom: 20px;
        }
        .table-container, .graph-container {
            background-color: #2B2B2B;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            border-radius: 8px;
        }
        .table-container {
            width: 48%;
            margin-right: 2%;
        }
        .graph-container {
            width: 48%;
        }
        .table-title, .graph-title {
            font-size: 24px;
            margin-bottom: 15px;
            text-align: center;
            color: #FFFFFF;
        }
        table {
            width: 100%;
            border-collapse: collapse;
        }
        th, td {
            padding: 12px;
            text-align: center;
            border-bottom: 1px solid #444;
            color: #FFFFFF;
        }
        th {
            background-color: #EC5800;
            color: #FFFFFF;
        }
        tr:hover {
            background-color: #444;
        }
        .status-operational {
            color: #00FF00; /* Green for Operational */
        }
        .status-error {
            color: #FF0000; /* Red for Error */
        }
        .status-on {
            color: #00FF00; /* Green for On */
        }
        .status-off {
            color: #FF0000; /* Red for Off */
        }
        .status-open {
            color: #00FF00; /* Green for Open */
        }
        .status-closed {
            color: #FF0000; /* Red for Closed */
        }
        @media (max-width: 600px) {
            th, td {
                font-size: 14px;
                padding: 8px;
            }
            .title {
                font-size: 28px;
            }
            .datetime {
                font-size: 16px;
            }
            .tables-container {
                flex-direction: column;
            }
            .table-container, .graph-container {
                width: 100%;
                margin-bottom: 20px;
            }
        }
    </style>
</head>
<body>
    <div class="header">
        <div class="title">
            <i class="fas fa-leaf"></i> <!-- Leaf Icon -->
            ThermoPlant
        </div>
        <div class="datetime" id="datetime">Date Time</div>
    </div>
    <hr>
    <div class="container">
        <!-- Tables and Graphs -->
        <div class="tables-container">
            <!-- Components Table -->
            <div class="table-container" id="table1">
                <div class="table-title">Components</div>
                <table id="componentTable">
                    <tr>
                        <th>ID</th>
                        <th>Type</th>
                        <th>Name</th>
                        <th>Pin</th>
                    </tr>
                </table>
            </div>
            <!-- Sensor Status Graph -->
            <div class="graph-container">
                <div class="graph-title">Sensor Status</div>
                <canvas id="sensorStatusChart"></canvas>
            </div>
        </div>

        <!-- Real Time Monitor Table and Temperature Graph -->
        <div class="tables-container">
            <!-- Real Time Monitor Table -->
            <div class="table-container" id="table2">
                <div class="table-title">Real Time Monitor</div>
                <table id="monitorTable">
                    <tr>
                        <th>Name</th>
                        <th>Value</th>
                        <th>Status</th>
                    </tr>
                </table>
            </div>

            <!-- Temperature Graph -->
            <div class="graph-container">
                <div class="graph-title">Temperature Values</div>
                <canvas id="temperatureChart"></canvas>
            </div>
        </div>
    </div>

    <script>
let sensorStatusChartInstance = null;
let temperatureChartInstance = null;

let sensorStatusData = [];
let temperatureData = [];

let sensorStatusLabels = [];
let temperatureLabels = [];

function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        return new XMLHttpRequest();
    } else {
        return new ActiveXObject("Microsoft.XMLHTTP");
    }
}

function getStatusClass(status, type) {
    if (type === 'Sensor') {
        return status === 'Operational' ? 'status-operational' : (status === 'ERROR' ? 'status-error' : '');
    } else if (type === 'Motor') {
        if (status === 'On') return 'status-on';
        if (status === 'Off') return 'status-off';
        if (status === 'Open') return 'status-open';
        if (status === 'Closed') return 'status-closed';
        return ''; // Default if status is something unexpected
    }
    return ''; // Default if type is not recognized
}

function loadComponentsAndSensorData() {
    var xmlHttp = createXmlHttpObject();
    xmlHttp.onreadystatechange = function() {
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
            console.log("Data fetched successfully");
            var data = xmlHttp.responseText;
            var parser = new DOMParser();
            var xmlDoc = parser.parseFromString(data, "text/xml");
            var sensors = xmlDoc.getElementsByTagName("Sensor");
            var motors = xmlDoc.getElementsByTagName("Motor");

            var componentTable = document.getElementById('componentTable');
            var monitorTable = document.getElementById('monitorTable');

            // Clear existing rows in both tables
            componentTable.innerHTML = `
                <tr>
                    <th>ID</th>
                    <th>Type</th>
                    <th>Name</th>
                    <th>Pin</th>
                </tr>
            `;
            monitorTable.innerHTML = `
                <tr>
                    <th>Name</th>
                    <th>Value</th>
                    <th>Status</th>
                </tr>
            `;

            // Get the current timestamp from the displayed date and time
            var currentTime = new Date(document.getElementById('datetime').textContent).toISOString();

            // Add rows for each sensor to the monitor table
            for (var i = 0; i < sensors.length; i++) {
                var name = sensors[i].getElementsByTagName("Name")[0].textContent;
                var value = sensors[i].getElementsByTagName("Value")[0].textContent;
                var status = sensors[i].getElementsByTagName("Status")[0].textContent;
                var id = sensors[i].getElementsByTagName("ID")[0].textContent;
                var type = sensors[i].getElementsByTagName("Type")[0].textContent;

                console.log(`Processing Sensor: ID=${id}, Name=${name}, Value=${value}, Status=${status}`);

                // Append to monitor table
                var row = document.createElement('tr');
                row.innerHTML = `
                    <td>${name}</td>
                    <td>${value}</td>
                    <td class="${getStatusClass(status, 'Sensor')}">${status}</td>
                `;
                monitorTable.appendChild(row);

                // Append to components table
                var pin = sensors[i].getElementsByTagName("Pin")[0].textContent;

                var compRow = document.createElement('tr');
                compRow.innerHTML = `
                    <td>${id}</td>
                    <td>${type}</td>
                    <td>${name}</td>
                    <td>${pin}</td>
                `;
                componentTable.appendChild(compRow);

                // Collect data for graphs
                if (type === 'Temperature') {
                    temperatureData.push(parseFloat(value));
                    temperatureLabels.push(new Date(currentTime).toLocaleTimeString());
                }

                sensorStatusData.push(status === 'Operational' ? 1 : 0);
                sensorStatusLabels.push(new Date(currentTime).toLocaleTimeString());
            }

            // Add rows for each motor to the monitor table
            for (var i = 0; i < motors.length; i++) {
                var name = motors[i].getElementsByTagName("Name")[0].textContent;
                var status = motors[i].getElementsByTagName("Status")[0].textContent;
                var id = motors[i].getElementsByTagName("ID")[0].textContent;
                var type = motors[i].getElementsByTagName("Type")[0].textContent;

                console.log(`Processing Motor: ID=${id}, Name=${name}, Status=${status}`);

                // Append to monitor table
                var row = document.createElement('tr');
                row.innerHTML = `
                    <td>${name}</td>
                    <td></td>
                    <td class="${getStatusClass(status, 'Motor')}">${status}</td>
                `;
                monitorTable.appendChild(row);

                // Append to components table
                var pin = motors[i].getElementsByTagName("Pin")[0].textContent;

                var compRow = document.createElement('tr');
                compRow.innerHTML = `
                    <td>${id}</td>
                    <td>${type}</td>
                    <td>${name}</td>
                    <td>${pin}</td>
                `;
                componentTable.appendChild(compRow);
            }

            // Update graphs
            updateGraphs(sensorStatusLabels, sensorStatusData, temperatureLabels, temperatureData);
        } else if (xmlHttp.readyState == 4) {
            console.error("Error fetching data. Status: " + xmlHttp.status);
        }
    };
    xmlHttp.open("GET", "/xml", true);
    xmlHttp.send();
}

function updateGraphs(sensorStatusLabels, sensorStatusData, temperatureLabels, temperatureData) {
    console.log("Updating graphs");

    var ctx1 = document.getElementById('sensorStatusChart').getContext('2d');
    var ctx2 = document.getElementById('temperatureChart').getContext('2d');

    if (!sensorStatusChartInstance) {
        // Initialize sensor status chart
        sensorStatusChartInstance = new Chart(ctx1, {
            type: 'line',
            data: {
                labels: sensorStatusLabels,
                datasets: [{
                    label: 'Sensor Status',
                    data: sensorStatusData,
                    borderColor: '#00FF00',
                    backgroundColor: 'rgba(0, 255, 0, 0.2)',
                    borderWidth: 2,
                    fill: false
                }]
            },
            options: {
                scales: {
                    y: {
                        min: 0,
                        max: 1,
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: 'Status (0=Error, 1=Operational)'
                        },
                        ticks: {
                            stepSize: 1,
                            callback: function(value) {
                                return value === 0 || value === 1 ? value : '';
                            }
                        }
                    },
                    x: {
                        title: {
                            display: true,
                            text: 'Time'
                        }
                    }
                }
            }
        });
    } else {
        // Update sensor status chart
        sensorStatusChartInstance.data.labels = sensorStatusLabels;
        sensorStatusChartInstance.data.datasets[0].data = sensorStatusData;
        sensorStatusChartInstance.update();
    }

    if (!temperatureChartInstance) {
        // Initialize temperature chart
        temperatureChartInstance = new Chart(ctx2, {
            type: 'line',
            data: {
                labels: temperatureLabels,
                datasets: [{
                    label: 'Temperature',
                    data: temperatureData,
                    borderColor: '#FF0000',
                    backgroundColor: 'rgba(255, 0, 0, 0.2)',
                    borderWidth: 2
                }]
            },
            options: {
                scales: {
                    y: {
                        title: {
                            display: true,
                            text: 'Temperature (°C)'
                        },
                        ticks: {
                            stepSize: 1,
                            callback: function(value) { return value; } // Show integers only
                        }
                    },
                    x: {
                        title: {
                            display: true,
                            text: 'Time'
                        }
                    }
                }
            }
        });
    } else {
        // Update temperature chart
        temperatureChartInstance.data.labels = temperatureLabels;
        temperatureChartInstance.data.datasets[0].data = temperatureData;
        temperatureChartInstance.update();
    }
}

function updateDateTime() {
    var now = new Date();
    var datetimeElement = document.getElementById('datetime');
    datetimeElement.textContent = now.toLocaleString();
}

updateDateTime();
setInterval(updateDateTime, 1000);

// Load components and sensor data initially and then update every second
loadComponentsAndSensorData();
setInterval(loadComponentsAndSensorData, 1000);

    </script>
</body>
</html>

)=====";
