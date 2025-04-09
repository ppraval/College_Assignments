
import { Header } from "@/components/Header";
import { PageTransition } from "@/components/PageTransition";
import { useAuth } from "@/context/AuthContext";
import { Sidebar } from "@/components/Sidebar";
import { 
  Server, 
  UserCog, 
  Bell, 
  FileText, 
  Cpu, 
  ShieldCheck, 
  PieChart, 
  Workflow 
} from "lucide-react";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

const Features = () => {
  const { isAuthenticated, userRole } = useAuth();
  
  // Convert userRole to the format expected by Sidebar component
  const sidebarRole = userRole === "student" ? "user" : (userRole || "user");
  
  // Feature items
  const features = [
    {
      title: "Server Management",
      description: "Monitor and manage all your server resources from a single dashboard.",
      icon: <Server className="h-10 w-10 text-blue-500" />
    },
    {
      title: "User Administration",
      description: "Manage user accounts, roles, and permissions with ease.",
      icon: <UserCog className="h-10 w-10 text-indigo-500" />
    },
    {
      title: "Request Handling",
      description: "Create, track, and manage resource requests efficiently.",
      icon: <FileText className="h-10 w-10 text-green-500" />
    },
    {
      title: "Issue Tracking",
      description: "Report and track issues with servers and resources in real-time.",
      icon: <Bell className="h-10 w-10 text-yellow-500" />
    },
    {
      title: "Resource Monitoring",
      description: "Get real-time insights into resource utilization and performance.",
      icon: <Cpu className="h-10 w-10 text-red-500" />
    },
    {
      title: "Security Controls",
      description: "Implement robust security measures to protect your infrastructure.",
      icon: <ShieldCheck className="h-10 w-10 text-purple-500" />
    },
    {
      title: "Analytics Dashboard",
      description: "Visualize data and generate reports to make informed decisions.",
      icon: <PieChart className="h-10 w-10 text-blue-600" />
    },
    {
      title: "Workflow Automation",
      description: "Automate routine tasks and streamline operations.",
      icon: <Workflow className="h-10 w-10 text-teal-500" />
    }
  ];
  
  return (
    <div className="min-h-screen bg-background">
      <Header />
      {isAuthenticated && <Sidebar role={sidebarRole} />}
      
      <PageTransition>
        <main className={`pt-24 pb-16 px-8 animate-fade-in ${isAuthenticated ? 'pl-72' : ''}`}>
          <div className="max-w-6xl mx-auto bg-blue-50/80 p-8 rounded-lg shadow-sm">
            <h1 className="text-3xl font-semibold mb-6">Key Features</h1>
            
            <p className="text-muted-foreground mb-8">
              Discover the powerful features that make CC Portal the ideal solution for managing your server infrastructure.
            </p>
            
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
              {features.map((feature, index) => (
                <Card key={index} className="border-border hover:shadow-md transition-shadow">
                  <CardHeader className="flex flex-row items-center gap-4 pb-2">
                    {feature.icon}
                    <CardTitle className="text-xl">{feature.title}</CardTitle>
                  </CardHeader>
                  <CardContent>
                    <p className="text-muted-foreground">{feature.description}</p>
                  </CardContent>
                </Card>
              ))}
            </div>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Features;
