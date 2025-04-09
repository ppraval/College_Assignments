
import { Navigate } from "react-router-dom";
import { useAuth } from "@/context/AuthContext";
import { PageTransition } from "@/components/PageTransition";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Checkbox } from "@/components/ui/checkbox";
import { useState } from "react";
import { ArrowLeft, Loader2, AlertTriangle } from "lucide-react";
import { useNavigate } from "react-router-dom";
import { toast } from "sonner";

const Login = () => {
  const { isAuthenticated, userRole, login, isLoading } = useAuth();
  const navigate = useNavigate();
  
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [rememberMe, setRememberMe] = useState(false);
  const [loginError, setLoginError] = useState("");

  // If already authenticated, redirect to appropriate dashboard
  if (isAuthenticated) {
    return <Navigate to={userRole === "admin" ? "/admin/dashboard" : "/user/dashboard"} />;
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoginError("");
    
    if (!username || !password) {
      toast.error("Please enter both username and password");
      return;
    }
    
    try {
      await login(username, password, rememberMe);
    } catch (error) {
      console.error("Login submission error:", error);
      if (error instanceof Error) {
        if (error.message.includes('Connection refused') || error.message.includes('Failed to fetch')) {
          setLoginError("Cannot connect to the server. Please make sure it's running at http://localhost:5000.");
        } else {
          setLoginError(error.message);
        }
      }
    }
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-50">
      <div className="container mx-auto p-4">
        <button 
          onClick={() => navigate("/")}
          className="mb-8 mt-6 flex items-center text-blue-600 hover:text-blue-800"
        >
          <ArrowLeft size={16} className="mr-2" />
          Back to home
        </button>
      </div>
      
      <PageTransition>
        <div className="flex flex-col items-center justify-center min-h-[80vh] px-4">
          <div className="w-full max-w-md p-8 space-y-8 bg-white rounded-lg shadow-lg border border-blue-100 animate-scale-in">
            <div className="text-center space-y-2">
              <h2 className="text-2xl font-semibold tracking-tight">Welcome Back</h2>
              <p className="text-sm text-muted-foreground">
                Sign in to access your Command Center
              </p>
            </div>

            <form className="space-y-6" onSubmit={handleSubmit}>
              <div className="space-y-2">
                <Label htmlFor="username">Username</Label>
                <Input
                  id="username"
                  placeholder="Enter Username"
                  value={username}
                  onChange={(e) => setUsername(e.target.value)}
                  className="w-full"
                  required
                />
              </div>
              
              <div className="space-y-2">
                <Label htmlFor="password">Password</Label>
                <Input
                  id="password"
                  type="password"
                  placeholder="Enter your password"
                  value={password}
                  onChange={(e) => setPassword(e.target.value)}
                  className="w-full"
                  required
                />
              </div>
              
              {loginError && (
                <div className="p-4 rounded bg-amber-50 text-amber-700 text-sm flex items-start">
                  <AlertTriangle className="h-5 w-5 mr-2 flex-shrink-0 mt-0.5" />
                  <div>
                    {loginError}
                    {loginError.includes("server") && (
                      <div className="mt-2 text-xs">
                        1. Make sure the server is running with <code className="bg-amber-100 p-1 rounded">npm run server</code><br/>
                        2. The server should be running on http://localhost:5000
                      </div>
                    )}
                  </div>
                </div>
              )}
              
              <div className="flex items-center space-x-2">
                <Checkbox 
                  id="remember-me" 
                  checked={rememberMe}
                  onCheckedChange={(checked) => setRememberMe(checked === true)}
                />
                <Label htmlFor="remember-me" className="text-sm cursor-pointer">
                  Remember me
                </Label>
              </div>
              
              <Button
                type="submit"
                className="w-full bg-gradient-to-r from-blue-600 to-indigo-600"
                disabled={isLoading}
              >
                {isLoading ? (
                  <>
                    <Loader2 className="mr-2 h-4 w-4 animate-spin" />
                    Signing in...
                  </>
                ) : "Login"}
              </Button>
              
              <div className="text-center text-sm text-muted-foreground">
                <p>
                  Try these test credentials:
                </p>
                <p className="mt-1 font-medium">
                  admin001 / admin123
                </p>
                <p className="font-medium">
                  student001 / user123
                </p>
              </div>
            </form>
          </div>
        </div>
      </PageTransition>
    </div>
  );
};

export default Login;
