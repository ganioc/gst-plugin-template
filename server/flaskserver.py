from flask import Flask, json
import time


companies = [{"id": 1, "name": "Company One"}, {"id": 2, "name": "Company Two"}]

api = Flask(__name__)

@api.route('/companies', methods=['GET'])
def get_companies():
  return json.dumps(companies)

@api.route('/one', methods=['GET'])
def get_one():
  T = time.localtime(time.time())
  return time.strftime("%Y-%m-%d %H:%M:%S",T)




@api.route('/two', methods=['GET'])
def get_two():
  return "32:12:12:12:39"

if __name__ == '__main__':
    api.run()