
import React, { createContext, useContext, useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { toast } from "sonner";

// Update UserRole type to include "student" since we've changed our database schema
export type UserRole = "admin" | "student" | null;

interface AuthContextType {
  isAuthenticated: boolean;
  userRole: UserRole;
  userID: string | null;
  userName: string | null;
  login: (username: string, password: string, rememberMe: boolean) => Promise<void>;
  logout: () => void;
  isLoading: boolean;
}

const AuthContext = createContext<AuthContextType | undefined>(undefined);

export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error("useAuth must be used within an AuthProvider");
  }
  return context;
};

const API_URL = import.meta.env.VITE_API_URL || 'http://localhost:5000/api';

export const AuthProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [isAuthenticated, setIsAuthenticated] = useState<boolean>(false);
  const [userRole, setUserRole] = useState<UserRole>(null);
  const [userID, setUserID] = useState<string | null>(null);
  const [userName, setUserName] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState<boolean>(false);
  const navigate = useNavigate();

  // Check localStorage on mount
  useEffect(() => {
    const token = localStorage.getItem("token");
    const savedRole = localStorage.getItem("userRole") as UserRole;
    const savedUserID = localStorage.getItem("userID");
    const savedUserName = localStorage.getItem("userName");
    
    if (token && savedRole && savedUserID) {
      setIsAuthenticated(true);
      setUserRole(savedRole);
      setUserID(savedUserID);
      setUserName(savedUserName);
    }
  }, []);

  const login = async (username: string, password: string, rememberMe: boolean) => {
    try {
      setIsLoading(true);
      
      console.log(`Attempting login for user: ${username}`);
      
      // Add timeout to the fetch request to prevent hanging
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 10000); // 10 second timeout
      
      const response = await fetch(`${API_URL}/auth/login`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ username, password }),
        signal: controller.signal
      }).catch(error => {
        if (error.name === 'AbortError') {
          throw new Error('Login request timed out. Please try again.');
        }
        throw new Error('Connection refused. Is the server running?');
      });
      
      clearTimeout(timeoutId);
      
      if (!response) {
        throw new Error('Could not connect to the server. Please check if it\'s running.');
      }
      
      const data = await response.json();
      
      if (!response.ok) {
        console.error('Login failed:', data);
        throw new Error(data.message || 'Login failed');
      }
      
      // Save token and user data in localStorage regardless of rememberMe
      // This ensures the token is available for API calls during the session
      localStorage.setItem("token", data.token);
      localStorage.setItem("userRole", data.user.role);
      localStorage.setItem("userID", data.user.userID);
      localStorage.setItem("userName", data.user.name);
      
      setIsAuthenticated(true);
      setUserRole(data.user.role);
      setUserID(data.user.userID);
      setUserName(data.user.name);
      
      // Redirect based on role
      navigate(data.user.role === "admin" ? "/admin/dashboard" : "/user/dashboard");
      
      toast.success(`Welcome, ${data.user.name}!`);
    } catch (error) {
      console.error('Login error:', error);
      let errorMessage = 'Authentication failed';
      
      if (error instanceof Error) {
        if (error.message.includes('Connection refused') || error.message.includes('Failed to fetch')) {
          errorMessage = 'Cannot connect to the server. Please ensure the server is running at ' + API_URL;
        } else {
          errorMessage = error.message;
        }
      }
      
      toast.error(errorMessage);
      throw error;
    } finally {
      setIsLoading(false);
    }
  };

  const logout = () => {
    setIsAuthenticated(false);
    setUserRole(null);
    setUserID(null);
    setUserName(null);
    localStorage.removeItem("token");
    localStorage.removeItem("userRole");
    localStorage.removeItem("userID");
    localStorage.removeItem("userName");
    navigate("/");
    toast.success("Logged out successfully");
  };

  return (
    <AuthContext.Provider value={{ 
      isAuthenticated, 
      userRole, 
      userID,
      userName,
      login, 
      logout, 
      isLoading 
    }}>
      {children}
    </AuthContext.Provider>
  );
};
