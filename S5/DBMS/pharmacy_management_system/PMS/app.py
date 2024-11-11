from flask import Flask, session, render_template, redirect, url_for
from flask import request
import pyodbc
from datetime import datetime, timedelta

app = Flask(__name__)
app.secret_key = 'PMS_Session_key'

# Database connection configuration
connection_string = 'DRIVER={ODBC Driver 18 for SQL Server};SERVER=praval.database.windows.net;DATABASE=pravaldb;UID=praval;PWD=ninja@123'


@app.route('/dashboard')
def dashboard():
    return render_template('dashboard.html')


#region login
@app.route('/', methods=['GET', 'POST'])
def index():
    username = request.form.get('username')
    user_password = request.form.get('user_password')

    if(username is not None and user_password is not None):
        user = get_user(username, user_password)
        if user is not None:
            session['role'] = user.role
            return redirect(url_for('dashboard'))

    return render_template('login.html')

# Function to get login user details
def get_user(username, user_password):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    query = 'select id, role from users where username = ? AND password = ?'
    cursor.execute(query, (username, user_password))
    user = cursor.fetchone()
    conn.close()
    return user
#endregion login

#region catalog
@app.route('/catalog')
def catalog():
    items = get_catalog()
    return render_template('catalog.html', items=items)

#endregion catalog

#region company
@app.route('/company')
def company():
    companies = get_companies()
    return render_template('company.html', companies=companies)
#endregion

#region buy
@app.route('/buy_company')
def buy_company():
    companies = get_companies()
    return render_template('buy_company.html', companies=companies)

@app.route('/buy_catalog/<int:company_id>')
def buy_catalog(company_id):
    company = get_company(company_id)
    catalog_items = get_catalog()
    return render_template('buy_catalog.html', company=company, catalog_items=catalog_items)

@app.route('/buy', methods=['POST'])
def buy():
    company_id = request.form['company_id']
    drug_code = request.form['drug_code']
    quantity = int(request.form['quantity'])

    catalog_item = get_catalog_details(drug_code)
    if not catalog_item:
        return "Drug not found in catalog", 404

    name, price, image_url = catalog_item

    insert_purchase(company_id, drug_code, quantity, price)

    if drug_exists(drug_code):
        update_buy_drug_quantity(drug_code, quantity)
    else:
        insert_drug(drug_code, name, company_id, quantity, price, image_url)

    return render_template('buy.html', name=name, image_url=image_url, quantity=quantity)


# Function to get all companies
def get_companies():
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT id, name, image_url FROM company')
    companies = cursor.fetchall()
    conn.close()
    return companies


# Function to get company details
def get_company(company_id: int):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT id, name, image_url FROM company WHERE id = ?', (company_id,))
    company = cursor.fetchone()
    conn.close()
    return company

# Function to get all catalog items
def get_catalog():
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT drug_code, name, price, image_url FROM catalogue')
    catalog = cursor.fetchall()
    conn.close()
    return catalog

# Function to get catalog details
def get_catalog_details(drug_code):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT name, price, image_url FROM catalogue WHERE drug_code = ?', (drug_code,))
    catalog_item = cursor.fetchone()
    conn.close()
    return catalog_item

# Function to check if drug_code exists in drugs table
def drug_exists(drug_code):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT COUNT(*) FROM drugs WHERE code = ?', (drug_code,))
    exists = cursor.fetchone()[0] > 0
    conn.close()
    return exists

# Function to update drug quantity
def update_buy_drug_quantity(drug_code, quantity):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('UPDATE drugs SET quantity = quantity + ? WHERE code = ?', (quantity, drug_code))
    conn.commit()
    conn.close()

# Function to insert into purchase table
def insert_purchase(company_id, drug_code, quantity, price):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    purchase_date = datetime.now()
    cursor.execute('INSERT INTO purchase (company_id, drug_code, quantity, price, date) VALUES (?, ?, ?, ?, ?)',
                   (company_id, drug_code, quantity, price, purchase_date))
    conn.commit()
    conn.close()

# Function to insert into drugs table
def insert_drug(drug_code, name, company_id, quantity, price, image_url):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    production_date = datetime.now()
    expiration_date = production_date + timedelta(days=365)
    cursor.execute('INSERT INTO drugs (code, name, company_id, quantity, cost_price, sell_price, production_date, expiration_date, image_url) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)',
                   (drug_code, name, company_id, quantity, price, price, production_date, expiration_date, image_url))
    conn.commit()
    conn.close()

#endregion buy

#region sale

@app.route('/sale')
def sale():
    drugs = get_all_drugs()
    return render_template('sale.html', drugs=drugs)

@app.route('/sell', methods=['POST'])
def sell():
    drug_code = request.form['drug_code']
    quantity = int(request.form['quantity'])
    selling_price = float(request.form['selling_price'])

    sale_id = insert_sale(drug_code, quantity, selling_price)
    insert_history_sale(sale_id, drug_code, quantity, selling_price)
    update_sell_drug_quantity(drug_code, quantity)

    drug = get_drug(drug_code)
    return render_template('sell.html', drug=drug, quantity=quantity)

# Function to get all drugs
def get_all_drugs():
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT code, name, quantity, sell_price, expiration_date, image_url FROM drugs')
    drugs = cursor.fetchall()
    conn.close()
    return drugs

def get_drug(drug_code):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT code, name, quantity, sell_price, expiration_date, image_url FROM drugs WHERE code = ?', (drug_code,))
    drug = cursor.fetchone()
    conn.close()
    return drug


# Function to insert into sales table
def insert_sale(drug_code, quantity, selling_price):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    sale_date = datetime.now()
    cursor.execute('INSERT INTO sales (drug_code, date, quantity, price) VALUES (?, ?, ?, ?)',
                   (drug_code, sale_date, quantity, selling_price))
    sale_id = cursor.execute('SELECT @@IDENTITY').fetchone()[0]
    conn.commit()
    conn.close()
    return sale_id

# Function to insert into history_sales table
def insert_history_sale(sale_id, drug_code, quantity, selling_price):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    sale_date = datetime.now()
    cursor.execute('INSERT INTO history_sales (sales_id, drug_code, date, quantity, price) VALUES (?, ?, ?, ?, ?)',
                   (sale_id, drug_code, sale_date, quantity, selling_price))
    conn.commit()
    conn.close()

# Function to update drug quantity
def update_sell_drug_quantity(drug_code, quantity):
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('UPDATE drugs SET quantity = quantity - ? WHERE code = ?', (quantity, drug_code))
    conn.commit()
    conn.close()

#endregion sale

#region Sales History
@app.route('/sales_history')
def sales_history():
    sales_history = get_sales_history()
    return render_template('sales_history.html', sales_history=sales_history)

# Function to get sales history data
def get_sales_history():
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    query = '''
    SELECT hs.date, hs.quantity, hs.price, d.name, d.image_url
    FROM history_sales hs
    JOIN drugs d ON hs.drug_code = d.code
    '''
    cursor.execute(query)
    sales_history = cursor.fetchall()
    conn.close()
    return sales_history
#endregion Sales History

# region inventory
@app.route('/inventory')
def inventory():
    inventory_items = get_inventory()
    return render_template('inventory.html', inventory_items=inventory_items)

def get_inventory():
    conn = pyodbc.connect(connection_string)
    cursor = conn.cursor()
    cursor.execute('SELECT name, expiration_date, image_url, quantity, sell_price FROM drugs')
    inventory = cursor.fetchall()
    conn.close()
    return inventory

#endregion inventory

if __name__ == '__main__':
    app.run()


