
-- Create the command_center database
CREATE DATABASE IF NOT EXISTS command_center;
USE command_center;

-- Create the users table (base User class)
CREATE TABLE IF NOT EXISTS users (
  userID VARCHAR(50) PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  role ENUM('admin', 'student') NOT NULL,
  email VARCHAR(100) NOT NULL,
  phone VARCHAR(20),
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  password VARCHAR(255) NOT NULL
);

-- Create the admin table (Admin subclass)
CREATE TABLE IF NOT EXISTS admins (
  userID VARCHAR(50) PRIMARY KEY,
  designation VARCHAR(100) NOT NULL,
  researchArea VARCHAR(100),
  FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE
);

-- Create the student table (Student subclass)
CREATE TABLE IF NOT EXISTS students (
  userID VARCHAR(50) PRIMARY KEY,
  department VARCHAR(100) NOT NULL,
  year INT NOT NULL,
  FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE
);

-- Create the admin_panel table
CREATE TABLE IF NOT EXISTS admin_panel (
  adminID VARCHAR(50) PRIMARY KEY,
  report TEXT,
  FOREIGN KEY (adminID) REFERENCES admins(userID) ON DELETE CASCADE
);

-- Create the server table
CREATE TABLE IF NOT EXISTS servers (
  serverID VARCHAR(50) PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  IPAddress VARCHAR(45) NOT NULL,
  status ENUM('online', 'offline', 'maintenance') DEFAULT 'offline',
  maintenanceSchedule DATETIME
);

-- Create the request table
CREATE TABLE IF NOT EXISTS requests (
  requestID VARCHAR(50) PRIMARY KEY,
  type VARCHAR(50) NOT NULL,
  description TEXT NOT NULL,
  status ENUM('pending', 'approved', 'rejected') DEFAULT 'pending',
  timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
  userID VARCHAR(50) NOT NULL,
  serverID VARCHAR(50),
  FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE,
  FOREIGN KEY (serverID) REFERENCES servers(serverID) ON DELETE SET NULL
);

-- Create the wifi_session table
CREATE TABLE IF NOT EXISTS wifi_sessions (
  sessionID VARCHAR(50) PRIMARY KEY,
  userID VARCHAR(50) NOT NULL,
  deviceMAC VARCHAR(50) NOT NULL,
  loginTime DATETIME NOT NULL,
  logoutTime DATETIME,
  FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE
);

-- Insert admin users
INSERT INTO users (userID, name, role, email, phone, password) VALUES 
('admin001', 'System Administrator', 'admin', 'admin@example.com', '123-456-7890', '$2a$08$kHYM0s7ywfyGDGfbXJ6I.uFu0T3eeAHPZ30yEtgYyIKK1KFWDZeC.'),
('admin002', 'John Smith', 'admin', 'john.admin@example.com', '123-456-7891', '$2a$08$kHYM0s7ywfyGDGfbXJ6I.uFu0T3eeAHPZ30yEtgYyIKK1KFWDZeC.'),
('admin003', 'Sarah Johnson', 'admin', 'sarah.admin@example.com', '123-456-7892', '$2a$08$kHYM0s7ywfyGDGfbXJ6I.uFu0T3eeAHPZ30yEtgYyIKK1KFWDZeC.');

-- Insert student users
INSERT INTO users (userID, name, role, email, phone, password) VALUES 
('student001', 'Alex Martinez', 'student', 'alex@example.com', '987-654-3210', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student002', 'Jane Wilson', 'student', 'jane@example.com', '987-654-3211', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student003', 'Mike Taylor', 'student', 'mike@example.com', '987-654-3212', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student004', 'Susan Brown', 'student', 'susan@example.com', '987-654-3213', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student005', 'Raj Patel', 'student', 'raj@example.com', '987-654-3214', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student006', 'Emily Chen', 'student', 'emily@example.com', '987-654-3215', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student007', 'David Kim', 'student', 'david@example.com', '987-654-3216', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student008', 'Lisa Johnson', 'student', 'lisa@example.com', '987-654-3217', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student009', 'Tom Peters', 'student', 'tom@example.com', '987-654-3218', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student010', 'Maria Rodriguez', 'student', 'maria@example.com', '987-654-3219', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student011', 'Emma Johnson', 'student', 'emma@example.com', '987-654-3220', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student012', 'Noah Williams', 'student', 'noah@example.com', '987-654-3221', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student013', 'Olivia Brown', 'student', 'olivia@example.com', '987-654-3222', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student014', 'Liam Jones', 'student', 'liam@example.com', '987-654-3223', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty'),
('student015', 'Ava Miller', 'student', 'ava@example.com', '987-654-3224', '$2a$08$SG1J1ZBVLU.ECETQnDqm6eBejdmjgsjHZD2q1CUG07HuI5LiH50Ty');

-- Insert into admins table
INSERT INTO admins (userID, designation, researchArea) VALUES
('admin001', 'Head Administrator', 'System Security'),
('admin002', 'Network Administrator', 'Network Management'),
('admin003', 'Database Administrator', 'Data Analytics');

-- Insert into students table
INSERT INTO students (userID, department, year) VALUES
('student001', 'Computer Science', 3),
('student002', 'Design', 2),
('student003', 'Research', 4),
('student004', 'Marketing', 1),
('student005', 'Engineering', 3),
('student006', 'Finance', 2),
('student007', 'Operations', 4),
('student008', 'Research', 3),
('student009', 'Engineering', 2),
('student010', 'Design', 1),
('student011', 'Computer Science', 2),
('student012', 'Physics', 3),
('student013', 'Mathematics', 1),
('student014', 'Engineering', 4),
('student015', 'Chemistry', 2);

-- Insert into admin_panel
INSERT INTO admin_panel (adminID, report) VALUES
('admin001', 'Monthly systems performance report'),
('admin002', 'Network traffic analysis for Q1'),
('admin003', 'Database optimization recommendations');

-- Insert sample servers
INSERT INTO servers (serverID, name, IPAddress, status, maintenanceSchedule) VALUES
('server001', 'Production Server 1', '192.168.1.101', 'online', '2023-12-15 02:00:00'),
('server002', 'Production Server 2', '192.168.1.102', 'online', '2023-12-22 02:00:00'),
('server003', 'Development Server', '192.168.1.103', 'online', '2023-12-10 08:00:00'),
('server004', 'Test Server', '192.168.1.104', 'maintenance', '2023-12-05 09:00:00'),
('server005', 'Backup Server', '192.168.1.105', 'online', '2023-12-18 01:00:00'),
('server006', 'Analytics Server', '192.168.1.106', 'online', '2023-12-29 03:00:00'),
('server007', 'Database Server', '192.168.1.107', 'online', '2023-12-12 02:30:00'),
('server008', 'Cache Server', '192.168.1.108', 'online', '2023-12-20 04:00:00'),
('server009', 'Load Balancer 1', '192.168.1.109', 'online', '2023-12-25 01:00:00'),
('server010', 'Load Balancer 2', '192.168.1.110', 'online', '2023-12-27 01:00:00');

-- Insert sample requests
INSERT INTO requests (requestID, type, description, status, timestamp, userID, serverID) VALUES
('req001', 'Access', 'Need access to the new database for project work', 'pending', '2023-11-05 09:30:00', 'student001', 'server007'),
('req002', 'Resource', 'VPN access to connect from home', 'approved', '2023-11-07 14:22:00', 'student002', NULL),
('req003', 'Software', 'Need MATLAB installed on workstation', 'pending', '2023-11-10 11:45:00', 'student003', 'server003'),
('req004', 'Hardware', 'Request for additional storage space', 'pending', '2023-11-15 08:15:00', 'student004', 'server005'),
('req005', 'Access', 'Database read permissions needed', 'rejected', '2023-11-18 16:30:00', 'student005', 'server007'),
('req006', 'Resource', 'GPU time for neural network training', 'approved', '2023-11-20 09:00:00', 'student006', 'server006'),
('req007', 'Software', 'Install PyTorch framework', 'pending', '2023-11-22 10:20:00', 'student007', 'server003'),
('req008', 'Access', 'GitHub Enterprise access', 'approved', '2023-11-25 13:45:00', 'student008', NULL),
('req009', 'Resource', 'Increased CPU allocation for simulation', 'pending', '2023-11-28 15:30:00', 'student009', 'server002'),
('req010', 'Hardware', 'Request for second monitor', 'rejected', '2023-11-30 11:15:00', 'student010', NULL);

-- Insert wifi session data
INSERT INTO wifi_sessions (sessionID, userID, deviceMAC, loginTime, logoutTime) VALUES
('wifi001', 'student001', '00:1A:2B:3C:4D:5E', '2023-12-01 08:30:00', '2023-12-01 10:45:00'),
('wifi002', 'student002', '00:2B:3C:4D:5E:6F', '2023-12-01 09:15:00', '2023-12-01 12:30:00'),
('wifi003', 'student003', '00:3C:4D:5E:6F:7G', '2023-12-01 10:00:00', '2023-12-01 15:20:00'),
('wifi004', 'admin001', '00:4D:5E:6F:7G:8H', '2023-12-01 08:00:00', '2023-12-01 17:00:00'),
('wifi005', 'student004', '00:5E:6F:7G:8H:9I', '2023-12-01 11:30:00', '2023-12-01 13:45:00'),
('wifi006', 'student005', '00:6F:7G:8H:9I:0J', '2023-12-01 12:15:00', NULL),
('wifi007', 'admin002', '00:7G:8H:9I:0J:1K', '2023-12-01 09:30:00', '2023-12-01 16:45:00'),
('wifi008', 'student006', '00:8H:9I:0J:1K:2L', '2023-12-01 14:00:00', '2023-12-01 16:30:00'),
('wifi009', 'student007', '00:9I:0J:1K:2L:3M', '2023-12-01 15:45:00', NULL),
('wifi010', 'student008', '00:0J:1K:2L:3M:4N', '2023-12-01 16:30:00', '2023-12-01 18:15:00');
