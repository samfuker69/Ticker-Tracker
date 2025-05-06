import yfinance as yf
from flask import Flask, jsonify
import threading
import time

#! Modify stock tickers or indices here
stocks = ['AAPL', 'AMD', 'AMZN', 'GOOGL', 'LULU', 'META', 'MSFT', 'NFLX', 'NVDA', 'PLTR', 'SMCI', 'TSLA']
index = ['^GSPC', '^DJI', '^IXIC', '^RUT', '^FTSE', '^GDAXI', '^FCHI', '^STOXX50E', '^N225', '^HSI', '000001.SS', '^AXJO']

#! Create a Flask web server to serve stock data API
app = Flask(__name__)
stock_data = {}
index_data = {}

def fetch_stock_data():
    global stocks, stock_data, index, index_data
    while True:
        new_stock_data = {}
        new_index_data = {}
        for symbol in stocks:
            info = []
            data = yf.Ticker(symbol)
            info.append(data.info['longName'])
            info.append(data.info['industry'])

            df = data.history(period="1d")
            current_price = df.iloc[0]['Close'].round(2)
            open_price = df.iloc[0]['Open'].round(2)
            dollar_change = current_price - open_price
            percent_change = ((dollar_change)/open_price*100).round(2)
            info.append(current_price)
            info.append(dollar_change)
            info.append(percent_change)
            new_stock_data[symbol] = info

        for symbol in index:
            info = []
            data = yf.Ticker(symbol)
            info.append(data.info['shortName'])
            info.append(data.info['exchange'])
            info.append(data.info['currency'])

            df = data.history(period="1d")
            current_price = df.iloc[0]['Close'].round(2)
            open_price = df.iloc[0]['Open'].round(2)
            dollar_change = current_price - open_price
            percent_change = ((dollar_change)/open_price*100).round(2)
            info.append(current_price)
            info.append(dollar_change)
            info.append(percent_change)
            new_index_data[symbol] = info

        stock_data = new_stock_data
        index_data = new_index_data
        time.sleep(60)  # Fetch data every minute

@app.route('/stocks', methods=['GET'])
def get_stock_data():
    print("Request received for stocks")
    return jsonify(stock_data)

@app.route('/indices', methods=['GET'])
def get_index_data():
    print("Request received for indices")
    return jsonify(index_data)

if __name__ == '__main__':
    threading.Thread(target=fetch_stock_data).start()
    app.run(host='0.0.0.0', port=5000)