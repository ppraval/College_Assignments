
import { useState } from "react";
import { Navigate } from "react-router-dom";
import { useAuth } from "@/context/AuthContext";
import { Header } from "@/components/Header";
import { PageTransition } from "@/components/PageTransition";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Checkbox } from "@/components/ui/checkbox";

const Index = () => {
  const { isAuthenticated, userRole, login } = useAuth();
  
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [rememberMe, setRememberMe] = useState(false);
  const [isLoading, setIsLoading] = useState(false);

  // If already authenticated, redirect to appropriate dashboard
  if (isAuthenticated) {
    return <Navigate to={userRole === "admin" ? "/admin/dashboard" : "/user/dashboard"} />;
  }

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setIsLoading(true);
    
    // Simulate network request
    setTimeout(() => {
      login(username, password, rememberMe);
      setIsLoading(false);
    }, 800);
  };

  return (
    <div className="min-h-screen bg-background">
      <Header />
      
      <PageTransition>
        <div className="flex flex-col items-center justify-center min-h-screen px-4">
          <div className="w-full max-w-md p-8 space-y-8 bg-white rounded-lg shadow-sm border border-border animate-scale-in">
            <div className="text-center space-y-2">
              <h2 className="text-2xl font-semibold tracking-tight">Login Panel</h2>
              <p className="text-sm text-muted-foreground">
                Sign in to access your account
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
                className="w-full"
                disabled={isLoading}
              >
                {isLoading ? "Signing in..." : "Login"}
              </Button>
              
              <div className="text-center text-sm text-muted-foreground">
                <p>
                  Try "admin" or "user" as username with any password
                </p>
              </div>
            </form>
          </div>
        </div>
      </PageTransition>
    </div>
  );
};

export default Index;
