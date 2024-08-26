import { WebSocket } from "ws";

class RTDBClient {
    private socket: WebSocket;

    private constructor(socket: WebSocket) {
        this.socket = socket;
    }

    public static connect(host: string): Promise<RTDBClient> {
        // Wait for client to connect
        return new Promise((resolve, reject) => {
            const socket = new WebSocket(host);
            socket.on("open", () => {
                resolve(new RTDBClient(socket));
            });
            socket.on("error", (code: number, reason: Buffer) => {
                reject(reason);
            });
        });
    }

    public send(message: string) {
        this.socket.send(message);
    }
    
    public close() {
        this.socket.close()
    }
}

(async () => {
    const client = await RTDBClient.connect("http://localhost:9002");
    
    client.send("hello");
})();
