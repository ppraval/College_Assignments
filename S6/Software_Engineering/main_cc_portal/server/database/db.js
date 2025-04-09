
const mysql = require('mysql2/promise');
require('dotenv').config();
const bcrypt = require('bcryptjs');

// Create a connection pool
const pool = mysql.createPool({
  host: process.env.DB_HOST || 'localhost',
  user: process.env.DB_USER || 'root',
  password: process.env.DB_PASSWORD || '',
  database: process.env.DB_NAME || 'command_center',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0
});

// Test database connection
async function testConnection() {
  try {
    const connection = await pool.getConnection();
    console.log('Database connection established successfully');
    
    // Create tables if they don't exist
    await createTables();
    
    connection.release();
    return true;
  } catch (error) {
    console.error('Database connection failed:', error);
    return false;
  }
}

// Create necessary tables if they don't exist
async function createTables() {
  try {
    // Create users table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS users (
        id INT AUTO_INCREMENT PRIMARY KEY,
        userID VARCHAR(50) UNIQUE NOT NULL,
        name VARCHAR(100) NOT NULL,
        role ENUM('admin', 'student') NOT NULL,
        email VARCHAR(100) UNIQUE NOT NULL,
        phone VARCHAR(20),
        password VARCHAR(255) NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
      )
    `);
    
    // Create admins table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS admins (
        id INT AUTO_INCREMENT PRIMARY KEY,
        userID VARCHAR(50) UNIQUE NOT NULL,
        designation VARCHAR(100),
        researchArea VARCHAR(100),
        FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE
      )
    `);
    
    // Create students table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS students (
        id INT AUTO_INCREMENT PRIMARY KEY,
        userID VARCHAR(50) UNIQUE NOT NULL,
        department VARCHAR(100),
        year INT,
        FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE
      )
    `);
    
    // Create servers table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS servers (
        serverID VARCHAR(50) PRIMARY KEY,
        name VARCHAR(100) NOT NULL,
        ip VARCHAR(50) NOT NULL,
        status ENUM('online', 'offline', 'maintenance') DEFAULT 'online',
        type VARCHAR(50),
        description TEXT,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
      )
    `);
    
    // Create requests table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS requests (
        requestID VARCHAR(50) PRIMARY KEY,
        userID VARCHAR(50) NOT NULL,
        serverID VARCHAR(50),
        type VARCHAR(50) NOT NULL,
        description TEXT,
        status ENUM('pending', 'approved', 'rejected') DEFAULT 'pending',
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (userID) REFERENCES users(userID) ON DELETE CASCADE,
        FOREIGN KEY (serverID) REFERENCES servers(serverID) ON DELETE SET NULL
      )
    `);
    
    // Create issues table if not exists
    await pool.execute(`
      CREATE TABLE IF NOT EXISTS issues (
        id INT AUTO_INCREMENT PRIMARY KEY,
        user_id VARCHAR(50) NOT NULL,
        server_id VARCHAR(50),
        title VARCHAR(100) NOT NULL,
        description TEXT,
        priority ENUM('High', 'Medium', 'Low') DEFAULT 'Medium',
        status ENUM('pending', 'resolved') DEFAULT 'pending',
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users(userID) ON DELETE CASCADE,
        FOREIGN KEY (server_id) REFERENCES servers(serverID) ON DELETE SET NULL
      )
    `);
    
    console.log('Tables created/verified successfully');
    
    // Check if we need to create some test servers
    const [serverRows] = await pool.execute('SELECT COUNT(*) as count FROM servers');
    if (serverRows[0].count < 1) {
      await createTestServers();
    }
    
    // Check if we need to create some test requests
    const [requestRows] = await pool.execute('SELECT COUNT(*) as count FROM requests');
    if (requestRows[0].count < 1) {
      await createTestRequests();
    }
    
    // Check if we need to create some test issues
    const [issueRows] = await pool.execute('SELECT COUNT(*) as count FROM issues');
    if (issueRows[0].count < 1) {
      await createTestIssues();
    }
    
    return true;
  } catch (error) {
    console.error('Error creating tables:', error);
    return false;
  }
}

// Create test servers
async function createTestServers() {
  try {
    const servers = [
      { serverID: 'srv001', name: 'Main Application Server', ip: '192.168.1.10', status: 'online', type: 'Production', description: 'Primary application server for the department' },
      { serverID: 'srv002', name: 'Database Server', ip: '192.168.1.11', status: 'online', type: 'Production', description: 'MySQL database server' },
      { serverID: 'srv003', name: 'Development Server', ip: '192.168.1.12', status: 'maintenance', type: 'Development', description: 'Server for development and testing' }
    ];
    
    for (const server of servers) {
      await pool.execute(
        'INSERT INTO servers (serverID, name, ip, status, type, description) VALUES (?, ?, ?, ?, ?, ?)',
        [server.serverID, server.name, server.ip, server.status, server.type, server.description]
      );
    }
    
    console.log('Test servers created successfully');
    return true;
  } catch (error) {
    console.error('Error creating test servers:', error);
    return false;
  }
}

// Create test issues
async function createTestIssues() {
  try {
    console.log('Creating test issues...');
    
    // Get user IDs
    const [userRows] = await pool.execute('SELECT userID FROM users LIMIT 3');
    if (userRows.length === 0) {
      console.log('No users found to create test issues');
      await createTestUsers();
      return false;
    }
    
    // Sample issues data
    const issues = [
      { 
        user_id: userRows[0].userID, 
        title: "Wi-Fi not working", 
        description: "Cannot connect to Campus Wi-Fi", 
        priority: "High"
      },
      { 
        user_id: userRows.length > 1 ? userRows[1].userID : userRows[0].userID, 
        title: "VPN Issue", 
        description: "Unable to connect to VPN from home", 
        priority: "Medium"
      },
      { 
        user_id: userRows.length > 2 ? userRows[2].userID : userRows[0].userID, 
        title: "Email Problems", 
        description: "Cannot send or receive emails since this morning", 
        priority: "High"
      }
    ];
    
    for (const issue of issues) {
      await pool.execute(
        'INSERT INTO issues (user_id, title, description, priority) VALUES (?, ?, ?, ?)',
        [issue.user_id, issue.title, issue.description, issue.priority]
      );
    }
    
    console.log('Test issues created successfully');
    return true;
  } catch (error) {
    console.error('Error creating test issues:', error);
    return false;
  }
}

// Create test requests - Completely rewriting this function to ensure it works
async function createTestRequests(forceCreate = false) {
  try {
    console.log('Creating test requests...');
    
    // Check if requests table exists, and if not, create tables first
    try {
      const [tableCheck] = await pool.execute('SHOW TABLES LIKE "requests"');
      if (tableCheck.length === 0) {
        console.log('Requests table does not exist, creating tables first');
        await createTables();
      }
    } catch (tableErr) {
      console.error('Error checking if requests table exists:', tableErr);
      // Create tables anyway
      await createTables();
    }
    
    // Check if we should create test data
    if (!forceCreate) {
      // If not forcing creation, check if there are already requests
      const [requestCount] = await pool.execute('SELECT COUNT(*) as count FROM requests');
      if (requestCount[0].count > 0) {
        console.log(`Found ${requestCount[0].count} existing requests, skipping test data creation`);
        return false;
      }
    }
    
    // Get user IDs
    const [userRows] = await pool.execute('SELECT userID FROM users LIMIT 10');
    if (userRows.length === 0) {
      console.log('No users found to create test requests');
      // Create test users first
      await createTestUsers();
      // Try again
      const [retryUsers] = await pool.execute('SELECT userID FROM users LIMIT 10');
      if (retryUsers.length === 0) {
        console.error('Still no users found after creating test users');
        return false;
      }
    }
    
    // Get server IDs
    let serverRows;
    try {
      [serverRows] = await pool.execute('SELECT serverID FROM servers LIMIT 3');
      if (serverRows.length === 0) {
        console.log('No servers found, creating test servers');
        await createTestServers();
        [serverRows] = await pool.execute('SELECT serverID FROM servers LIMIT 3');
      }
    } catch (e) {
      console.error('Error fetching servers:', e);
      serverRows = [];
    }
    
    // Clear existing requests if forcing recreation
    if (forceCreate) {
      try {
        console.log('Forcing test data recreation - clearing existing requests');
        await pool.execute('DELETE FROM requests');
        console.log('Existing requests cleared');
      } catch (clearErr) {
        console.error('Error clearing existing requests:', clearErr);
      }
    }
    
    // Create sample requests
    const testUsers = await pool.execute('SELECT userID FROM users LIMIT 10');
    const userIDs = testUsers[0].map(u => u.userID);
    
    // Sample requests data
    const sampleRequests = [
      {
        requestID: 'req001',
        userID: userIDs[0] || 'student001',
        serverID: serverRows[0]?.serverID || null,
        type: 'High',
        description: 'Access to GPU server for machine learning project',
        status: 'pending',
      },
      {
        requestID: 'req002',
        userID: userIDs[1] || userIDs[0] || 'student001',
        serverID: serverRows[0]?.serverID || null,
        type: 'Medium',
        description: 'Additional storage space for design assets',
        status: 'approved',
      },
      {
        requestID: 'req003',
        userID: userIDs[2] || userIDs[0] || 'student001',
        serverID: serverRows[1]?.serverID || serverRows[0]?.serverID || null,
        type: 'Low',
        description: 'Software installation request for data analysis tools',
        status: 'rejected',
      },
      {
        requestID: 'req004',
        userID: userIDs[0] || 'student001',
        serverID: serverRows[1]?.serverID || null,
        type: 'High',
        description: 'Database access for research project',
        status: 'pending',
      },
      {
        requestID: 'req005',
        userID: userIDs[3] || userIDs[0] || 'student001',
        serverID: serverRows[0]?.serverID || null,
        type: 'Medium',
        description: 'Cloud resources for marketing campaign analysis',
        status: 'approved',
      },
      {
        requestID: 'req006',
        userID: userIDs[1] || userIDs[0] || 'student001',
        serverID: serverRows[2]?.serverID || null,
        type: 'High',
        description: 'Need access to web server for hosting project',
        status: 'pending',
      },
      {
        requestID: 'req007',
        userID: userIDs[4] || userIDs[0] || 'student001',
        serverID: serverRows[0]?.serverID || null,
        type: 'Low',
        description: 'Request for data backup storage increase',
        status: 'pending',
      },
      {
        requestID: 'req008',
        userID: userIDs[2] || userIDs[0] || 'student001',
        serverID: serverRows[1]?.serverID || null,
        type: 'Medium',
        description: 'Need to install specialized software for research',
        status: 'approved',
      }
    ];
    
    // Insert test requests
    let insertedCount = 0;
    for (const req of sampleRequests) {
      try {
        // Check if this request already exists
        const [existingReq] = await pool.execute('SELECT requestID FROM requests WHERE requestID = ?', [req.requestID]);
        
        if (existingReq.length === 0 || forceCreate) {
          // Use REPLACE INTO to handle both insert and update
          await pool.execute(
            'REPLACE INTO requests (requestID, userID, serverID, type, description, status) VALUES (?, ?, ?, ?, ?, ?)',
            [req.requestID, req.userID, req.serverID, req.type, req.description, req.status]
          );
          insertedCount++;
          console.log(`Created/updated test request: ${req.requestID}`);
        } else {
          console.log(`Request ${req.requestID} already exists, skipping`);
        }
      } catch (reqErr) {
        console.error(`Error creating test request ${req.requestID}:`, reqErr);
      }
    }
    
    console.log(`${insertedCount} test requests processed successfully`);
    return true;
  } catch (error) {
    console.error('Error creating test requests:', error);
    return false;
  }
}

// Get user by username (userID)
async function getUserByUsername(username) {
  try {
    const [rows] = await pool.execute('SELECT * FROM users WHERE userID = ?', [username]);
    return rows[0] || null;
  } catch (error) {
    console.error('Error finding user:', error);
    return null;
  }
}

// Get user with role-specific info
async function getUserWithRoleInfo(userID) {
  try {
    // First get the base user info
    const [userRows] = await pool.execute('SELECT * FROM users WHERE userID = ?', [userID]);
    
    if (!userRows[0]) return null;
    
    const user = userRows[0];
    
    // Then get the role-specific info
    if (user.role === 'admin') {
      const [adminRows] = await pool.execute('SELECT * FROM admins WHERE userID = ?', [userID]);
      if (adminRows[0]) {
        return { ...user, ...adminRows[0] };
      }
    } else if (user.role === 'student') {
      const [studentRows] = await pool.execute('SELECT * FROM students WHERE userID = ?', [userID]);
      if (studentRows[0]) {
        return { ...user, ...studentRows[0] };
      }
    }
    
    return user;
  } catch (error) {
    console.error('Error finding user with role info:', error);
    return null;
  }
}

// Create test users if they don't exist
async function createTestUsers() {
  try {
    // Check if admin user exists
    const adminExists = await getUserByUsername('admin001');
    if (!adminExists) {
      // Use a lower cost factor (8) for bcrypt to generate shorter hashes
      const adminPassword = await bcrypt.hash('admin123', 8);
      await pool.execute(
        'INSERT INTO users (userID, name, role, email, phone, password) VALUES (?, ?, ?, ?, ?, ?)',
        ['admin001', 'System Administrator', 'admin', 'admin@example.com', '123-456-7890', adminPassword]
      );
      
      await pool.execute(
        'INSERT INTO admins (userID, designation, researchArea) VALUES (?, ?, ?)',
        ['admin001', 'Head Administrator', 'System Security']
      );
      
      console.log('Created admin test user');
    } else {
      // Update the existing admin password for testing
      const adminPassword = await bcrypt.hash('admin123', 8);
      await pool.execute(
        'UPDATE users SET password = ? WHERE userID = ?',
        [adminPassword, 'admin001']
      );
      console.log('Updated admin test user password');
    }
    
    // Check if student user exists
    const userExists = await getUserByUsername('student001');
    if (!userExists) {
      const userPassword = await bcrypt.hash('user123', 8);
      await pool.execute(
        'INSERT INTO users (userID, name, role, email, phone, password) VALUES (?, ?, ?, ?, ?, ?)',
        ['student001', 'Alex Martinez', 'student', 'alex@example.com', '987-654-3210', userPassword]
      );
      
      await pool.execute(
        'INSERT INTO students (userID, department, year) VALUES (?, ?, ?)',
        ['student001', 'Computer Science', 3]
      );
      
      console.log('Created student test user');
    } else {
      // Update the existing student password for testing
      const userPassword = await bcrypt.hash('user123', 8);
      await pool.execute(
        'UPDATE users SET password = ? WHERE userID = ?',
        [userPassword, 'student001']
      );
      console.log('Updated student test user password');
    }
    
    // Create additional dummy users for testing if they don't exist
    const testUserCount = await countUsers();
    if (testUserCount < 15) {
      console.log('Creating additional test users...');
      await createDummyUsers();
    }
    
    return true;
  } catch (error) {
    console.error('Error creating test users:', error);
    return false;
  }
}

// Count users in the database
async function countUsers() {
  try {
    const [rows] = await pool.execute('SELECT COUNT(*) as count FROM users');
    return rows[0].count;
  } catch (error) {
    console.error('Error counting users:', error);
    return 0;
  }
}

// Create dummy users for testing
async function createDummyUsers() {
  try {
    const dummyUsers = [
      { userID: 'student011', name: 'Emma Johnson', role: 'student', email: 'emma@example.com', department: 'Computer Science', year: 2 },
      { userID: 'student012', name: 'Noah Williams', role: 'student', email: 'noah@example.com', department: 'Physics', year: 3 },
      { userID: 'student013', name: 'Olivia Brown', role: 'student', email: 'olivia@example.com', department: 'Mathematics', year: 1 },
      { userID: 'student014', name: 'Liam Jones', role: 'student', email: 'liam@example.com', department: 'Engineering', year: 4 },
      { userID: 'student015', name: 'Ava Miller', role: 'student', email: 'ava@example.com', department: 'Chemistry', year: 2 },
      { userID: 'admin004', name: 'Isabella Martinez', role: 'admin', email: 'isabella@example.com', designation: 'Security Admin', researchArea: 'Cybersecurity' },
      { userID: 'admin005', name: 'Ethan Wilson', role: 'admin', email: 'ethan@example.com', designation: 'Network Admin', researchArea: 'Network Architecture' }
    ];
    
    const password = await bcrypt.hash('password123', 8);
    
    for (const user of dummyUsers) {
      // Check if user already exists
      const userExists = await getUserByUsername(user.userID);
      if (!userExists) {
        // Insert into users table
        await pool.execute(
          'INSERT INTO users (userID, name, role, email, password) VALUES (?, ?, ?, ?, ?)',
          [user.userID, user.name, user.role, user.email, password]
        );
        
        // Insert role-specific data
        if (user.role === 'admin') {
          await pool.execute(
            'INSERT INTO admins (userID, designation, researchArea) VALUES (?, ?, ?)',
            [user.userID, user.designation, user.researchArea]
          );
        } else if (user.role === 'student') {
          await pool.execute(
            'INSERT INTO students (userID, department, year) VALUES (?, ?, ?)',
            [user.userID, user.department, user.year]
          );
        }
        
        console.log(`Created dummy user: ${user.userID}`);
      }
    }
    
    return true;
  } catch (error) {
    console.error('Error creating dummy users:', error);
    return false;
  }
}

// Get all users with pagination and filtering
async function getUsers(limit = 20, offset = 0, role = null) {
  try {
    console.log('Getting users with params:', { limit, offset, role });
    
    // Simplified query to troubleshoot database connection
    let query = 'SELECT * FROM users';
    const params = [];
    
    if (role) {
      query += ' WHERE role = ?';
      params.push(role);
    }
    
    // For debugging, no pagination for now to simplify
    console.log('Executing simplified query:', query);
    console.log('With params:', params);
    
    const [rows] = await pool.execute(query, params);
    console.log(`Query returned ${rows.length} users`);
    
    // Log first user details for debugging (excluding password)
    if (rows.length > 0) {
      const { password, ...firstUser } = rows[0];
      console.log('First user:', firstUser);
    }
    
    // Check if users are being returned correctly
    // Process the results to include role-specific details
    const processedUsers = [];
    for (const user of rows) {
      // Exclude password from response
      const { password, ...safeUser } = user;
      
      // Get role-specific details
      if (user.role === 'admin') {
        try {
          const [adminRows] = await pool.execute('SELECT designation FROM admins WHERE userID = ?', [user.userID]);
          if (adminRows[0]) {
            safeUser.detail = adminRows[0].designation;
          }
        } catch (error) {
          console.error(`Error getting admin details for ${user.userID}:`, error);
        }
      } else if (user.role === 'student') {
        try {
          const [studentRows] = await pool.execute('SELECT department FROM students WHERE userID = ?', [user.userID]);
          if (studentRows[0]) {
            safeUser.detail = studentRows[0].department;
          }
        } catch (error) {
          console.error(`Error getting student details for ${user.userID}:`, error);
        }
      }
      
      processedUsers.push(safeUser);
    }
    
    // Add pagination back once query works
    if (limit) {
      return processedUsers.slice(offset, offset + limit);
    }
    
    return processedUsers;
  } catch (error) {
    console.error('Error getting users:', error);
    return [];
  }
}

// Get all servers
async function getServers() {
  try {
    const [rows] = await pool.execute('SELECT * FROM servers ORDER BY name');
    return rows;
  } catch (error) {
    console.error('Error getting servers:', error);
    return [];
  }
}

// Update server status
async function updateServerStatus(serverID, status) {
  try {
    const query = 'UPDATE servers SET status = ? WHERE serverID = ?';
    const [result] = await pool.execute(query, [status, serverID]);
    
    return result.affectedRows > 0;
  } catch (error) {
    console.error('Error updating server status:', error);
    return false;
  }
}

// Get all issues with pagination
async function getIssues(limit = 20, offset = 0, status = null) {
  try {
    let query = `
      SELECT i.*, u.name as userName 
      FROM issues i 
      JOIN users u ON i.user_id = u.userID
    `;
    
    const params = [];
    
    if (status) {
      query += ' WHERE i.status = ?';
      params.push(status);
    }
    
    query += ' ORDER BY i.priority DESC, i.created_at DESC LIMIT ? OFFSET ?';
    params.push(parseInt(limit), parseInt(offset));
    
    const [rows] = await pool.execute(query, params);
    return rows;
  } catch (error) {
    console.error('Error getting issues:', error);
    return [];
  }
}

// Update issue status
async function updateIssueStatus(issueID, status) {
  try {
    const query = 'UPDATE issues SET status = ? WHERE id = ?';
    const [result] = await pool.execute(query, [status, issueID]);
    
    return result.affectedRows > 0;
  } catch (error) {
    console.error('Error updating issue status:', error);
    return false;
  }
}

// Get requests with pagination and filtering
async function getRequests(userID = null, limit = 20, offset = 0, status = null) {
  try {
    // First, check if the requests table exists
    const [tableCheck] = await pool.execute('SHOW TABLES LIKE "requests"');
    if (tableCheck.length === 0) {
      console.error('Requests table does not exist');
      // Create tables first
      await createTables();
      // Try to create test data
      await createTestRequests(true);
    }
    
    // Get the count of requests for debugging
    const [countResult] = await pool.execute('SELECT COUNT(*) as count FROM requests');
    console.log(`Total requests in database: ${countResult[0].count}`);
    
    // Log user ID filtering if any
    if (userID) {
      console.log(`Filtering requests for user: ${userID}`);
      // Check if this user has any requests
      const [userRequestCount] = await pool.execute(
        'SELECT COUNT(*) as count FROM requests WHERE userID = ?',
        [userID]
      );
      console.log(`User ${userID} has ${userRequestCount[0].count} requests`);
    }
    
    let query = 'SELECT r.*, u.name as userName, s.name as serverName ' +
                'FROM requests r ' +
                'LEFT JOIN users u ON r.userID = u.userID ' +
                'LEFT JOIN servers s ON r.serverID = s.serverID';
    
    const params = [];
    let conditions = [];
    
    if (userID) {
      conditions.push('r.userID = ?');
      params.push(userID);
    }
    
    if (status) {
      conditions.push('r.status = ?');
      params.push(status);
    }
    
    if (conditions.length > 0) {
      query += ' WHERE ' + conditions.join(' AND ');
    }
    
    query += ' ORDER BY r.timestamp DESC';
    
    // Fix: Instead of using parameterized LIMIT/OFFSET, add them directly to the query string
    if (limit) {
      query += ` LIMIT ${parseInt(limit)} OFFSET ${parseInt(offset)}`;
    }
    
    console.log('Executing requests query:', query);
    console.log('With params:', params);
    
    // Execute the actual query without LIMIT/OFFSET parameters
    const [rows] = await pool.execute(query, params);
    console.log(`Query returned ${rows.length} requests`);
    
    // Log the first result for debugging
    if (rows.length > 0) {
      console.log('First request:', JSON.stringify(rows[0]));
    } else {
      console.log('No requests found for query');
      
      // If we got no results and we're not already filtering by user, try looking at all requests
      if (!userID && !status) {
        const [allRequests] = await pool.execute('SELECT * FROM requests LIMIT 10');
        console.log(`Debug - found ${allRequests.length} total requests in table`);
        if (allRequests.length > 0) {
          console.log('Sample request:', JSON.stringify(allRequests[0]));
        }
      }
    }
    
    return rows;
  } catch (error) {
    console.error('Error getting requests:', error);
    return [];
  }
}

// Update request status
async function updateRequestStatus(requestID, status) {
  try {
    const query = 'UPDATE requests SET status = ? WHERE requestID = ?';
    const [result] = await pool.execute(query, [status, requestID]);
    
    return result.affectedRows > 0;
  } catch (error) {
    console.error('Error updating request status:', error);
    return false;
  }
}

// Get wifi sessions for a user
async function getWifiSessions(userID = null, limit = 20, offset = 0) {
  try {
    let query = 'SELECT * FROM wifi_sessions';
    const params = [];
    
    if (userID) {
      query += ' WHERE userID = ?';
      params.push(userID);
    }
    
    query += ' ORDER BY loginTime DESC LIMIT ? OFFSET ?';
    params.push(limit, offset);
    
    const [rows] = await pool.execute(query, params);
    return rows;
  } catch (error) {
    console.error('Error getting wifi sessions:', error);
    return [];
  }
}

module.exports = {
  pool,
  testConnection,
  createTables,
  getUserByUsername,
  getUserWithRoleInfo,
  getUsers,
  getServers,
  updateServerStatus,
  getIssues,
  updateIssueStatus,
  getRequests,
  updateRequestStatus,
  getWifiSessions,
  createTestUsers,
  createTestRequests,
  createTestServers,
  createTestIssues
};
