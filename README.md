# Ticker Tracker

A real-time stock price indicator using an ESP32 microcontroller, powered by a Python backend running on a Raspberry Pi. The ESP32 fetches stock price data from a local Flask server and lights up **green** or **red** based on the performance of tracked tickers. The ESP32 also displays stock information such as company name, industry, and price on a connected LCD screen.

---

## 🔧 Project Overview

This project consists of two main components:

### 🐍 Python Server (`ticker_tracker_server.py`)
- Runs on a Raspberry Pi
- Uses the [`yfinance`](https://pypi.org/project/yfinance/) API to fetch live stock and index data
- Hosts a local Flask web server with two endpoints:
  - `/stocks`: serves stock information
  - `/indices`: serves major market indices
- Updates data every 60 seconds in a background thread

### ⚡ ESP32 Microcontroller (`cpp_ticker_tracker_stocks.ino`) or (`cpp_ticker_tracker_indices.ino`)
- Connects to Wi-Fi and queries the Raspberry Pi server
- Parses JSON stock data
- Displays company name, industry, and price info on a connected LCD screen
- Uses RGB LEDs to reflect stock performance:
  - **Green** → stock is up
  - **Red** → stock is down


---

## 📦 Folder Structure

```
ticker-tracker/
├── esp32-client/
|   └──stocks
|       └── wifi-security (WPA or WPA2)
│             └── cpp_ticker_tracker_stocks.ino
|   └──indices
|       └── wifi-security (WPA or WPA2)
│             └── cpp_ticker_tracker_indices.ino
├── python-server/
│   └── ticker_tracker_server.py
├── requirements.txt
└── README.md
```

---

## ⚙️ How It Works

1. The **Python server** uses `yfinance` to get live prices for selected tickers and indices.
2. It calculates:
   - Dollar change (intraday)
   - Percent change (intraday)
3. The data is served as JSON via HTTP.
4. The **ESP32** connects to the server, reads the JSON, and:
   - Displays stock information on its LCD screen
   - Lights the RGB LED accordingly
   

---

## 🚀 Getting Started

### Raspberry Pi (Python Server)

#### Install dependencies:
```bash
# Create a virtual environment (recommended)
python3 -m venv venv
source venv/bin/activate
# Install the required Python packages
pip install -r requirements.txt
```

#### Run the server:
```bash
python ticker_tracker_server.py
```
Server will be available at `http://<raspberry-pi-ip>:5000/stocks`

> You can optionally run it as a service or a `cron` job.

---

### ESP32 Microcontroller

#### Prerequisites:
- Arduino IDE or PlatformIO
- ESP32 board support installed
- Connect RGB LED to a digital pin (GND, 3.3v, Red=5, Green=18)
- Connect and configure LCD screen (GND, 3.3v, SDA=21, SCL=22)

#### Setup:
- In `cpp_ticker_tracker_stocks.ino`, configure:
  - Wi-Fi SSID and password
  - Raspberry Pi IP address

#### Flash:
Upload the code via USB.

---

## 🧪 Example API Response

### `/stocks`
```json
{
  "AAPL": ["Apple Inc.", "Consumer Electronics", 183.45, 1.25, 0.69],
  ...
}
```

### `/indices`
```json
{
  "^GSPC": ["S&P 500", "SNP", "USD", 4987.12, 15.23, 0.31],
  ...
}
```

Fields:
- `[0]`: Name
- `[1]`: Industry / Exchange
- `[2]`: Current price
- `[3]`: Dollar change
- `[4]`: Percent change

---

## 📌 Tickers Tracked

**Stocks**:
`AAPL`, `AMD`, `AMZN`, `GOOGL`, `LULU`, `META`, `MSFT`, `NFLX`, `NVDA`, `PLTR`, `SMCI`, `TSLA`

**Indices**:
`^GSPC`, `^DJI`, `^IXIC`, `^RUT`, `^FTSE`, `^GDAXI`, `^FCHI`, `^STOXX50E`, `^N225`, `^HSI`, `000001.SS`, `^AXJO`

---

## 📃 License

© 2025 Samuel Dion