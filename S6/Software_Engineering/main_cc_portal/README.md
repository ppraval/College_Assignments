
# Command Center Portal

A web application for server management, monitoring, and resource allocation with user and admin roles.

## Project Structure

- Frontend: React application with TypeScript, Tailwind CSS, and Shadcn UI components
- Backend: Node.js/Express server with MySQL database

## Setup Instructions

### Frontend Setup

1. Install dependencies:
```
npm install
```

2. Create a `.env` file with the following content:
```
VITE_API_URL=http://localhost:5000/api
```

3. Start the development server:
```
npm run dev
```

### Backend Setup

1. Navigate to the server directory:
```
cd server
```

2. Install dependencies:
```
npm install
```

3. Create a `.env` file based on the `.env.example` and configure your MySQL connection:
```
DB_HOST=localhost
DB_USER=root
DB_PASSWORD=your_mysql_password
DB_NAME=command_center
JWT_SECRET=your_secret_key_here
PORT=5000
```

4. Create a MySQL database by running the initialization script:
```
mysql -u root -p < database/init.sql
```

Or you can execute the SQL commands in the `server/database/init.sql` file using a MySQL client.

5. Start the server:
```
npm run dev
```

## Default Users

The system comes with two default users:

1. Admin User:
   - Username: admin
   - Password: admin123
   - Role: admin

2. Regular User:
   - Username: user
   - Password: user123
   - Role: user

## Features

- Role-based authentication (admin/user)
- Dashboard for both admin and user roles
- Server management and monitoring
- Resource allocation tracking
- User management (admin only)
- Request and issue tracking

## Technologies Used

- Frontend: React, TypeScript, Tailwind CSS, Shadcn UI
- Backend: Node.js, Express, MySQL
- Authentication: JWT (JSON Web Tokens)
