import { RawData, WebSocket } from "ws";

enum RTDBMessageType {
    SUBSCRIBE_ALL="SUBSCRIBE_ALL",
    PUSH_MESSAGE="PUSH",
    QUERY_STORE="QUERY"
}

class RTDBClient {
    private socket: WebSocket;

    private constructor(socket: WebSocket) {
        this.socket = socket;
        this.socket.on("message", (data: RawData, isBinary: boolean) => {
            console.log(data);
        })
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

    // Subscribes the clients to all updates from the given storeID
    public subscribeAll(storeID: string) {

    }

    private send(message: string) {
        this.socket.send(message);
    }
    
    public close() {
        this.socket.close()
    }
}

(async () => {
    const client = await RTDBClient.connect("http://localhost:9002");
    
    client.subscribeAll("chats")

    client.close();
})();

export { RTDBClient, RTDBMessageType };
