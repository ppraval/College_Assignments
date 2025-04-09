
import { useAuth } from "@/context/AuthContext";
import { cn } from "@/lib/utils";
import { UserCircle, Settings, LogOut, User } from "lucide-react";
import { Button } from "@/components/ui/button";
import { useState, useEffect } from "react";
import { Link } from "react-router-dom";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu";
import { Avatar, AvatarFallback } from "@/components/ui/avatar";

interface HeaderProps {
  className?: string;
}

export function Header({ className }: HeaderProps) {
  const { isAuthenticated, userRole, logout } = useAuth();
  const [scrolled, setScrolled] = useState(false);

  // Handle scroll effect
  useEffect(() => {
    const handleScroll = () => {
      setScrolled(window.scrollY > 10);
    };
    
    window.addEventListener("scroll", handleScroll);
    return () => window.removeEventListener("scroll", handleScroll);
  }, []);

  // Get user initial for avatar
  const getUserInitial = () => {
    return userRole ? userRole[0].toUpperCase() : "U";
  };

  return (
    <header
      className={cn(
        "fixed top-0 left-0 right-0 px-4 py-3 flex items-center justify-between z-50 transition-all duration-300",
        scrolled ? "bg-white/80 backdrop-blur-md shadow-sm" : "bg-white",
        className
      )}
    >
      <div className="flex items-center space-x-4">
        <h1 className="text-xl font-semibold">Main CC Portal</h1>
      </div>
      
      <div className="flex items-center space-x-4">
        {!isAuthenticated ? (
          <>
            <Button variant="ghost" size="sm" asChild>
              <Link to="/about">About</Link>
            </Button>
            <Button variant="ghost" size="sm" asChild>
              <Link to="/features">Features</Link>
            </Button>
          </>
        ) : (
          <>
            <Button variant="ghost" size="sm" asChild>
              <Link to="/about">About</Link>
            </Button>
            <Button variant="ghost" size="sm" asChild>
              <Link to="/features">Features</Link>
            </Button>
          </>
        )}
        
        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <button className="w-9 h-9 flex items-center justify-center rounded-full bg-muted overflow-hidden transition-transform hover:scale-105">
              {isAuthenticated ? (
                <Avatar className="h-9 w-9">
                  <AvatarFallback className="bg-primary text-primary-foreground">
                    {getUserInitial()}
                  </AvatarFallback>
                </Avatar>
              ) : (
                <UserCircle className="w-7 h-7 text-muted-foreground" />
              )}
            </button>
          </DropdownMenuTrigger>
          
          <DropdownMenuContent align="end" className="w-56">
            {isAuthenticated ? (
              <>
                <DropdownMenuLabel>
                  {userRole === 'admin' ? 'Administrator' : 'User'} Account
                </DropdownMenuLabel>
                <DropdownMenuSeparator />
                <DropdownMenuItem className="cursor-pointer">
                  <User className="mr-2 h-4 w-4" />
                  <span>My Profile</span>
                </DropdownMenuItem>
                <DropdownMenuItem className="cursor-pointer">
                  <Settings className="mr-2 h-4 w-4" />
                  <span>Settings</span>
                </DropdownMenuItem>
                <DropdownMenuSeparator />
                <DropdownMenuItem className="cursor-pointer text-destructive" onClick={logout}>
                  <LogOut className="mr-2 h-4 w-4" />
                  <span>Logout</span>
                </DropdownMenuItem>
              </>
            ) : (
              <DropdownMenuItem asChild>
                <Link to="/" className="cursor-pointer w-full">Login</Link>
              </DropdownMenuItem>
            )}
          </DropdownMenuContent>
        </DropdownMenu>
      </div>
    </header>
  );
}
