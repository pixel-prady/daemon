import WebSocket from 'ws';

const wss = new WebSocket.Server({ port: 9001 });
let daemonMap = {};
let uiClients = [];

wss.on('connection', (ws) => {
    console.log("New connection established.");

    ws.on('message', (message) => {
        try {
            const parsedMessage = message.split(':');
            const command = parsedMessage[0];
            const data = parsedMessage[1] || '';

            if (command === 'daemon') {
                const daemonId = data;
                if (!daemonId) {
                    ws.send(JSON.stringify({ error: "Daemon ID is required." }));
                    ws.close();
                    return;
                }
                daemonMap[daemonId] = ws;
                console.log(`Daemon ${daemonId} connected.`);
            } else if (command === 'update') {
                if (data) {
                    const [key, value] = data.split('=');
                    if (key && value) {
                        console.log(`Updating ${key} with value ${value}`);
                        ws.send(JSON.stringify({ success: true, message: `Updated ${key} to ${value}` }));
                    } else {
                        ws.send(JSON.stringify({ error: "Invalid format. Use update:key=value" }));
                    }
                } else {
                    ws.send(JSON.stringify({ error: "Invalid update command format. Expected 'key=value'" }));
                }
            } else if (command === 'ui') {
                const { daemonId, request } = JSON.parse(data);
                if (daemonId && daemonMap[daemonId]) {
                    daemonMap[daemonId].send(request);
                } else {
                    ws.send(JSON.stringify({
                        error: `Daemon ${daemonId} not found.`
                    }));
                    ws.close();
                }
            } else {
                ws.send(JSON.stringify({ error: "Unknown command" }));
            }
        } catch (error) {
            console.error("Error handling message:", error);
            ws.send(JSON.stringify({ error: "Failed to process the message" }));
        }
    });

    ws.on('close', () => {
        removeDaemon(ws);
        removeUIClient(ws);
        console.log("Connection closed.");
    });
});

function removeDaemon(ws) {
    Object.keys(daemonMap).forEach((daemonId) => {
        if (daemonMap[daemonId] === ws) {
            delete daemonMap[daemonId];
            console.log(`Daemon ${daemonId} disconnected.`);
        }
    });
}

function removeUIClient(ws) {
    uiClients = uiClients.filter(client => client !== ws);
    console.log("UI client disconnected.");
}

wss.on('connection', (ws) => {
    uiClients.push(ws);
    console.log("UI client connected.");

    ws.on('message', (message) => {
        console.log('Message from UI:', message);
    });

    ws.on('close', () => {
        removeUIClient(ws);
        console.log("UI client disconnected.");
    });
});

console.log("WebSocket server running on ws://localhost:9001");
