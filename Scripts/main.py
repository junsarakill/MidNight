from fastapi import FastAPI
from fastapi.responses import JSONResponse
import uvicorn

app = FastAPI()

@app.post("/throwing")
def throwing_value(data: dict):
    print("Received data:", data)
    return JSONResponse(content={"status": "success", "received_value": data})
@app.post("/eating")
def eating_value(data: dict):
    print("Received data:", data)
    return JSONResponse(content={"status": "success", "received_value": data})

@app.post("/cleaning")
def cleaning_value(data: dict):
    print("Received data:", data)
    return JSONResponse(content={"status": "success", "received_value": data})

@app.post("/giving")
def giving_value(data: dict):
    print("Received data:", data)
    return JSONResponse(content={"status": "success", "received_value": data})

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)