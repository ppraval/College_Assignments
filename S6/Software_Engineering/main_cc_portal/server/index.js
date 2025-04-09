const express = require('express');
const cors = require('cors');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
require('dotenv').config();
const { 
  pool, 
  testConnection, 
  getUserByUsername, 
  getUsers, 
  createTestUsers, 
  getRequests,
  updateRequestStatus,
  createTables,
  createTestRequests
} = require('./database/db');

const app = express();
const PORT = process.env.PORT || 5000;

// Middleware
app.use(cors());
app.use(express.json());

// Initialize database tables
async function initDatabase() {
  try {
    await testConnection();
    await createTables();
    // Create test users after connection is established
    await createTestUsers();
    // Create test requests to ensure there's data
    await createTestRequests(true);
    console.log('Database initialized successfully with test data');
  } catch (error) {
    console.error('Database initialization error:', error);
  }
}

// Auth route for login
app.post('/api/auth/login', async (req, res) => {
  try {
    const { username, password } = req.body;
    
    if (!username || !password) {
      return res.status(400).json({ message: 'Username and password are required' });
    }
    
    // Check if the user exists
    const user = await getUserByUsername(username);
    
    if (!user) {
      console.log(`Login attempt failed: User "${username}" not found`);
      return res.status(401).json({ message: 'Invalid credentials' });
    }
    
    // Log the stored hashed password for debugging
    console.log(`Stored password for ${username}: ${user.password.substring(0, 20)}...`);
    console.log(`Password attempt: ${password}`);
    
    const isPasswordValid = await bcrypt.compare(password, user.password);
    
    if (!isPasswordValid) {
      console.log(`Login attempt failed: Invalid password for user "${username}"`);
      return res.status(401).json({ message: 'Invalid credentials' });
    }
    
    // Generate JWT token
    const token = jwt.sign(
      { 
        id: user.id, 
        username: user.userID, 
        role: user.role,
        name: user.name  
      },
      process.env.JWT_SECRET || 'your_jwt_secret',
      { expiresIn: '24h' }
    );
    
    console.log(`Login successful for user "${username}" with role "${user.role}"`);
    
    res.status(200).json({
      message: 'Login successful',
      token,
      user: {
        userID: user.userID,
        username: user.userID,
        role: user.role,
        name: user.name
      }
    });
  } catch (error) {
    console.error('Login error:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// Verify token middleware
const verifyToken = (req, res, next) => {
  const authHeader = req.headers.authorization;
  
  if (!authHeader) {
    console.log('No authorization header provided');
    return res.status(401).json({ message: 'No token provided' });
  }
  
  const token = authHeader.split(' ')[1];
  
  if (!token) {
    console.log('Invalid authorization header format');
    return res.status(401).json({ message: 'No token provided' });
  }
  
  try {
    console.log(`Verifying token: ${token.substring(0, 20)}...`);
    const decoded = jwt.verify(token, process.env.JWT_SECRET || 'your_jwt_secret');
    req.user = decoded;
    console.log('Token verified for user:', decoded.username);
    next();
  } catch (error) {
    console.error('Token verification error:', error.name, error.message);
    
    if (error.name === 'TokenExpiredError') {
      return res.status(401).json({ message: 'Token expired' });
    }
    
    if (error.name === 'JsonWebTokenError') {
      return res.status(401).json({ message: 'Invalid token' });
    }
    
    return res.status(401).json({ message: 'Token verification failed' });
  }
};

// Admin middleware
const isAdmin = (req, res, next) => {
  if (req.user.role !== 'admin') {
    console.log(`Access denied for user ${req.user.username} with role ${req.user.role}`);
    return res.status(403).json({ message: 'Access denied. Admin privileges required.' });
  }
  console.log(`Admin access granted for user ${req.user.username}`);
  next();
};

// Protected route - User profile
app.get('/api/user/profile', verifyToken, (req, res) => {
  res.status(200).json({ user: req.user });
});

// PUBLIC API route to get all users - NO AUTH REQUIRED
app.get('/api/users', async (req, res) => {
  try {
    const limit = parseInt(req.query.limit) || 20;
    const offset = parseInt(req.query.offset) || 0;
    const role = req.query.role;
    
    console.log('Fetching users with public access. Params:', { limit, offset, role });
    const users = await getUsers(limit, offset, role);
    
    // Remove sensitive fields from each user for security
    const safeUsers = users.map(user => {
      // Ensure password is never sent to the client
      const { password, ...safeUser } = user;
      return safeUser;
    });
    
    console.log(`Found ${safeUsers.length} users`);
    res.status(200).json(safeUsers);
  } catch (error) {
    console.error('Error fetching users:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// Protected API route - Add a new user (no authentication required)
app.post('/api/users', async (req, res) => {
  try {
    const { userID, name, email, role, password, phone } = req.body;
    
    console.log('Adding user:', { userID, name, email, role, phone });
    
    // Validate required fields
    if (!userID || !name || !email || !role || !password) {
      return res.status(400).json({ message: 'All fields are required' });
    }
    
    // Check if user already exists
    const existingUser = await getUserByUsername(userID);
    if (existingUser) {
      return res.status(409).json({ message: 'Username already exists' });
    }
    
    // Hash the password
    const hashedPassword = await bcrypt.hash(password, 8);
    
    // Insert into users table
    const [result] = await pool.execute(
      'INSERT INTO users (userID, name, role, email, phone, password, created_at) VALUES (?, ?, ?, ?, ?, ?, NOW())',
      [userID, name, role, email, phone || null, hashedPassword]
    );
    
    // Insert role-specific information
    if (role === 'admin') {
      await pool.execute(
        'INSERT INTO admins (userID, designation, researchArea) VALUES (?, ?, ?)',
        [userID, 'New Administrator', null]
      );
    } else if (role === 'student') {
      await pool.execute(
        'INSERT INTO students (userID, department, year) VALUES (?, ?, ?)',
        [userID, 'General', 1]
      );
    }
    
    res.status(201).json({ 
      message: 'User created successfully',
      user: { userID, name, email, role, phone }
    });
  } catch (error) {
    console.error('Error creating user:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// Protected route - Delete a user (admin only)
app.delete('/api/users/:userID', verifyToken, isAdmin, async (req, res) => {
  try {
    const { userID } = req.params;
    
    // Prevent deleting yourself
    if (userID === req.user.username) {
      return res.status(400).json({ message: 'You cannot delete your own account' });
    }
    
    // Check if user exists
    const user = await getUserByUsername(userID);
    if (!user) {
      return res.status(404).json({ message: 'User not found' });
    }
    
    // Delete from users table (will cascade to role-specific tables)
    const [result] = await pool.execute('DELETE FROM users WHERE userID = ?', [userID]);
    
    if (result.affectedRows === 0) {
      return res.status(404).json({ message: 'User not found' });
    }
    
    res.status(200).json({ message: 'User deleted successfully' });
  } catch (error) {
    console.error('Error deleting user:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to get servers - Updated to be public for demo purposes
app.get('/api/servers', async (req, res) => {
  try {
    const [servers] = await pool.execute('SELECT * FROM servers');
    res.status(200).json(servers);
  } catch (error) {
    console.error('Error fetching servers:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to add a new server
app.post('/api/servers', async (req, res) => {
  try {
    const { serverID, name, ip, status, description } = req.body;
    
    console.log('Received server data:', req.body);
    
    if (!name || !ip) {
      return res.status(400).json({ message: 'Server name and IP address are required' });
    }
    
    // Generate serverID if not provided
    const serverId = serverID || `srv${Date.now().toString().slice(-5)}`;
    
    // Check if server already exists with this IP - Using IPAddress column instead of ip
    const [existingServer] = await pool.execute('SELECT serverID FROM servers WHERE IPAddress = ?', [ip]);
    if (existingServer.length > 0) {
      return res.status(409).json({ message: 'Server already exists with this IP address' });
    }
    
    // Insert into servers table - Using IPAddress column instead of ip
    // Only include fields that exist in the database schema
    const [result] = await pool.execute(
      'INSERT INTO servers (serverID, name, IPAddress, status) VALUES (?, ?, ?, ?)',
      [serverId, name, ip, status || 'offline']
    );
    
    console.log('Server created successfully:', serverId);
    
    res.status(201).json({ 
      message: 'Server created successfully',
      serverID: serverId,
      name,
      ipAddress: ip,
      status: status || 'offline'
    });
  } catch (error) {
    console.error('Error creating server:', error);
    res.status(500).json({ message: 'Server error', error: error.message });
  }
});

// API route to update server status
app.put('/api/servers/:serverID', async (req, res) => {
  try {
    const { serverID } = req.params;
    const { status } = req.body;
    
    if (!serverID || !status) {
      return res.status(400).json({ message: 'Server ID and status are required' });
    }
    
    // Update server status
    const [result] = await pool.execute(
      'UPDATE servers SET status = ? WHERE serverID = ?',
      [status, serverID]
    );
    
    if (result.affectedRows === 0) {
      return res.status(404).json({ message: 'Server not found' });
    }
    
    res.status(200).json({ 
      message: 'Server status updated successfully',
      serverID,
      status 
    });
  } catch (error) {
    console.error('Error updating server status:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// New endpoint to manually initialize test requests data - NO AUTH REQUIRED
app.post('/api/requests/init', async (req, res) => {
  try {
    console.log('Manually initializing test requests data');
    
    // First ensure tables exist
    await createTables();
    
    // Force creation of test requests
    await createTestRequests(true);
    
    res.status(200).json({ message: 'Test requests data initialized successfully' });
  } catch (error) {
    console.error('Error initializing test requests data:', error);
    res.status(500).json({ message: 'Server error', error: error.message });
  }
});

// API route to get requests - NO AUTH REQUIRED
app.get('/api/requests', async (req, res) => {
  try {
    const limit = parseInt(req.query.limit) || 100; // Increased limit for testing
    const offset = parseInt(req.query.offset) || 0;
    const status = req.query.status;
    
    console.log(`Fetching all requests without authentication`);
    
    // Force database tables initialization to ensure requests exist
    await createTables();
    
    // Try to create test data if needed
    try {
      const [requestCount] = await pool.execute('SELECT COUNT(*) as count FROM requests');
      console.log(`Current request count in database: ${requestCount[0].count}`);
      
      if (requestCount[0].count === 0) {
        console.log('No requests found - creating test requests');
        await createTestRequests(true);
      }
    } catch (err) {
      console.error('Error checking request count:', err);
    }
    
    // Get all requests without filtering by user
    const requests = await getRequests(null, limit, offset, status);
    console.log(`Returning ${requests.length} requests`);
    
    if (requests.length === 0) {
      console.log('No requests found after query - trying to create more test data');
      await createTestRequests(true);
      // Try fetching again
      const retryRequests = await getRequests(null, limit, offset, status);
      console.log(`After retry: returning ${retryRequests.length} requests`);
      return res.status(200).json(retryRequests);
    }
    
    res.status(200).json(requests);
  } catch (error) {
    console.error('Error fetching requests:', error);
    res.status(500).json({ message: 'Server error', error: error.message });
  }
});

// API route to update request status - NO AUTH REQUIRED
app.put('/api/requests/:requestID', async (req, res) => {
  try {
    const { requestID } = req.params;
    const { status } = req.body;
    
    if (!requestID || !status) {
      return res.status(400).json({ message: 'Request ID and status are required' });
    }
    
    if (!['approved', 'rejected', 'pending'].includes(status)) {
      return res.status(400).json({ message: 'Invalid status value' });
    }
    
    console.log(`Updating request ${requestID} status to ${status}`);
    const success = await updateRequestStatus(requestID, status);
    
    if (!success) {
      return res.status(404).json({ message: 'Request not found or not updated' });
    }
    
    res.status(200).json({ message: `Request ${requestID} status updated to ${status}` });
  } catch (error) {
    console.error('Error updating request status:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to create a new request
app.post('/api/requests', async (req, res) => {
  try {
    const { type, description, priority, userID } = req.body;
    
    if (!type || !description || !priority) {
      return res.status(400).json({ message: 'Type, description, and priority are required' });
    }
    
    // Generate a request ID
    const requestID = `req${Date.now()}`;
    
    // Use provided userID or default to a test user
    const requestUserID = userID || 'student001';
    
    console.log(`Creating new request for user ${requestUserID}:`, { type, description, priority });
    
    // Insert into requests table
    const [result] = await pool.execute(
      'INSERT INTO requests (requestID, userID, type, description, status) VALUES (?, ?, ?, ?, ?)',
      [requestID, requestUserID, type, description, 'pending']
    );
    
    res.status(201).json({ 
      message: 'Request created successfully',
      request: { requestID, userID: requestUserID, type, description, status: 'pending' }
    });
  } catch (error) {
    console.error('Error creating request:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to get issues
app.get('/api/issues', async (req, res) => {
  try {
    // For demo purposes, create a default issues table if it doesn't exist
    try {
      await pool.execute(`
        CREATE TABLE IF NOT EXISTS issues (
          id INT AUTO_INCREMENT PRIMARY KEY,
          user_id VARCHAR(50) NOT NULL,
          server_id VARCHAR(50),
          title VARCHAR(100) NOT NULL,
          description TEXT,
          priority ENUM('High', 'Medium', 'Low') DEFAULT 'Medium',
          status ENUM('pending', 'resolved', 'open', 'solved') DEFAULT 'open',
          created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
          FOREIGN KEY (user_id) REFERENCES users(userID) ON DELETE CASCADE,
          FOREIGN KEY (server_id) REFERENCES servers(serverID) ON DELETE SET NULL
        )
      `);
      
      // Check if we need to create some test issues
      const [issueRows] = await pool.execute('SELECT COUNT(*) as count FROM issues');
      if (issueRows[0].count < 1) {
        // Get some user IDs
        const [userRows] = await pool.execute('SELECT userID FROM users LIMIT 3');
        
        if (userRows.length > 0) {
          const testIssues = [
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
          
          for (const issue of testIssues) {
            await pool.execute(
              'INSERT INTO issues (user_id, title, description, priority) VALUES (?, ?, ?, ?)',
              [issue.user_id, issue.title, issue.description, issue.priority]
            );
          }
        }
      }
    } catch (tableErr) {
      console.error('Error setting up issues table:', tableErr);
    }
    
    // Get all issues with user names
    const [issues] = await pool.execute(`
      SELECT i.*, u.name as username 
      FROM issues i 
      JOIN users u ON i.user_id = u.userID 
      ORDER BY i.priority DESC, i.created_at DESC
    `);
    
    res.status(200).json(issues);
  } catch (error) {
    console.error('Error fetching issues:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to add a new issue
app.post('/api/issues', async (req, res) => {
  try {
    const { title, description, priority, user_id } = req.body;
    
    if (!title || !description || !priority) {
      return res.status(400).json({ message: 'Title, description, and priority are required' });
    }
    
    // Use provided userID or default to a test user
    const issueUserID = user_id || 'student001';
    
    console.log(`Creating new issue for user ${issueUserID}:`, { title, description, priority });
    
    // Insert into issues table
    const [result] = await pool.execute(
      'INSERT INTO issues (user_id, title, description, priority) VALUES (?, ?, ?, ?)',
      [issueUserID, title, description, priority]
    );
    
    res.status(201).json({ 
      message: 'Issue created successfully',
      issue: { id: result.insertId, title, description, priority, status: 'open', user_id: issueUserID }
    });
  } catch (error) {
    console.error('Error creating issue:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to update issue status
app.put('/api/issues/:issueID', async (req, res) => {
  try {
    const { issueID } = req.params;
    const { status } = req.body;
    
    if (!issueID || !status) {
      return res.status(400).json({ message: 'Issue ID and status are required' });
    }
    
    // Update issue status
    const [result] = await pool.execute(
      'UPDATE issues SET status = ? WHERE id = ?',
      [status, issueID]
    );
    
    if (result.affectedRows === 0) {
      return res.status(404).json({ message: 'Issue not found' });
    }
    
    res.status(200).json({ 
      message: 'Issue status updated successfully',
      issueID,
      status 
    });
  } catch (error) {
    console.error('Error updating issue status:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// API route to get resource allocations
app.get('/api/allocations', verifyToken, async (req, res) => {
  try {
    let query = 'SELECT ra.*, u.username, s.name as server_name FROM resource_allocations ra ' +
                'JOIN users u ON ra.user_id = u.id ' +
                'JOIN servers s ON ra.server_id = s.id';
    const params = [];
    
    // If user role is not admin, only show their allocations
    if (req.user.role !== 'admin') {
      query += ' WHERE ra.user_id = ?';
      params.push(req.user.id);
    }
    
    query += ' ORDER BY ra.allocation_start DESC';
    
    const [allocations] = await pool.execute(query, params);
    res.status(200).json(allocations);
  } catch (error) {
    console.error('Error fetching allocations:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

// Initialize database and start server
initDatabase().then(() => {
  app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
  });
});
