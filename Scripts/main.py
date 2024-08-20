from fastapi import FastAPI

app = FastAPI()

@app.post("/throwing")
def receive_value(data: dict):
    print("Received data:", data)
    return {"status": "success", "received_value": data}

@app.post("/eating")
def receive_value(data: dict):
    print("Received data:", data)
    return {"status": "success", "received_value": data}

@app.post("/cleaning")
def receive_value(data: dict):
    print("Received data:", data)
    return {"status": "success", "received_value": data}

@app.post("/giving")
def receive_value(data: dict):
    print("Received data:", data)
    return {"status": "success", "received_value": data}