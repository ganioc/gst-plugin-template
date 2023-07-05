from flask import Flask, json

companies = [{"id": 1, "name": "Company One"}, {"id": 2, "name": "Company Two"}]

api = Flask(__name__)

@api.route('/companies', methods=['GET'])
def get_companies():
  return json.dumps(companies)

@api.route('/one', methods=['GET'])
def get_one():
  return "32:12:12"

@api.route('/two', methods=['GET'])
def get_one():
  return "32:12:12:12:39"

if __name__ == '__main__':
    api.run()