
import { useNavigate, useLocation } from "react-router-dom";
import { cn } from "@/lib/utils";
import { 
  LayoutDashboard, 
  Users, 
  FileText, 
  AlertCircle, 
  Server
} from "lucide-react";

interface SidebarProps {
  role: "admin" | "user";
  className?: string;
}

interface NavItem {
  title: string;
  path: string;
  icon: React.ElementType;
}

export function Sidebar({ role, className }: SidebarProps) {
  const navigate = useNavigate();
  const location = useLocation();
  
  const basePath = role === "admin" ? "/admin" : "/user";
  
  const navItems: NavItem[] = [
    {
      title: "Dashboard",
      path: `${basePath}/dashboard`,
      icon: LayoutDashboard,
    },
    ...(role === "admin" 
      ? [
          {
            title: "Users",
            path: `${basePath}/users`,
            icon: Users,
          },
        ] 
      : []
    ),
    {
      title: "Requests",
      path: `${basePath}/requests`,
      icon: FileText,
    },
    {
      title: "Issues",
      path: `${basePath}/issues`,
      icon: AlertCircle,
    },
    {
      title: "Servers",
      path: `${basePath}/servers`,
      icon: Server,
    },
  ];

  return (
    <aside className={cn(
      "w-64 min-h-screen bg-background border-r border-border fixed top-0 left-0 pt-16 overflow-y-auto animate-fade-in",
      className
    )}>
      <div className="p-4 pt-6">
        <h2 className="text-lg font-semibold mb-6">{role === "admin" ? "Admin Panel" : "User Panel"}</h2>
        
        <nav className="space-y-1">
          {navItems.map((item) => (
            <button
              key={item.path}
              onClick={() => navigate(item.path)}
              className={cn(
                "w-full flex items-center gap-3 px-4 py-3 text-sm rounded-md transition-colors",
                location.pathname === item.path
                  ? "bg-primary text-primary-foreground"
                  : "text-muted-foreground hover:bg-muted"
              )}
            >
              <item.icon className="w-5 h-5" />
              <span>{item.title}</span>
            </button>
          ))}
        </nav>
      </div>
    </aside>
  );
}
