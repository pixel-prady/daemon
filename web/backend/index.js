import WebSocket, { WebSocketServer } from "ws";

const port = process.env.PORT || 8080;
const wss = new WebSocketServer({ port });


const daemonMap = new Map();

const subscriptions = new Map();

const uiDaemonMap = new Map();

console.log(`server started at port ${port}`);

wss.on("connection", (ws) => {
  ws.once("message", (message) => {
    const msg = message.toString().trim();

    if (msg.startsWith("daemonId:")) {
      const parts = msg.split(":");
      const daemonId = parts[1]?.trim();

      if (!daemonId) {
        ws.send("error: Daemon ID is required");
        ws.close();
        return;
      }

      daemonMap.set(daemonId, ws);
      subscriptions.set(daemonId, new Set());

      ws.send(`success: Daemon ${daemonId} registered`);
      console.log(`Daemon connected: ${daemonId}`);

      ws.on("message", (data) => {
        const uis = subscriptions.get(daemonId);
        if (uis) {
          uis.forEach((uiSocket) => {
            if (uiSocket.readyState === WebSocket.OPEN) {
              uiSocket.send(data);
            }
          });
        }
      });

      ws.on("close", () => {
        daemonMap.delete(daemonId);

        const uis = subscriptions.get(daemonId);
        if (uis) {
          uis.forEach((uiSocket) => {
            if (uiSocket.readyState === WebSocket.OPEN) {
              uiSocket.send(` Daemon disconnected plz connect again `);
              uiSocket.close();
            }
          });
          subscriptions.delete(daemonId);
        }
        console.log(`Daemon disconnected: ${daemonId}`);
      });
    } else {
      const daemonId = msg;

      if (!daemonMap.has(daemonId)) {
        ws.send(`error: Daemon ${daemonId} not found. Please try again later.`);
        ws.close();
        return;
      }

      if (!subscriptions.has(daemonId)) {
        subscriptions.set(daemonId, new Set());
      }
      subscriptions.get(daemonId).add(ws);
      uiDaemonMap.set(ws, daemonId);

      ws.send(
        `success: Connected to daemon ${daemonId}. You can now send commands and see real time logs .`
      );

      ws.on("message", (data) => {
        const daemonSocket = daemonMap.get(daemonId);

        if (!daemonSocket || daemonSocket.readyState !== WebSocket.OPEN) {
          ws.send("error: Daemon disconnected. Closing connection.");
          ws.close();
          return;
        }

        daemonSocket.send(data);
      });

      ws.on("close", () => {
        const daemonId = uiDaemonMap.get(ws);
        uiDaemonMap.delete(ws);

        if (daemonId && subscriptions.has(daemonId)) {
          subscriptions.get(daemonId).delete(ws);
          console.log(`UI disconnected from daemon ${daemonId}`);
        }
      });
    }
  });
});
