
import { useState, useEffect } from "react";
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { StatusBadge, StatusType } from "@/components/StatusBadge";
import { Spinner } from "@/components/ui/spinner";
import { toast } from "sonner";

interface Server {
  serverID: string;
  name: string;
  ipAddress: string;
  status: StatusType;
}

const Servers = () => {
  const [accessibleServers, setAccessibleServers] = useState<Server[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  
  useEffect(() => {
    fetchServers();
  }, []);
  
  const fetchServers = async () => {
    try {
      setIsLoading(true);
      const response = await fetch('http://localhost:5000/api/servers');
      
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      
      const data = await response.json();
      
      // Transform server data to match our interface
      const formattedServers = data.map((server: any) => ({
        serverID: server.serverID,
        name: server.name,
        ipAddress: server.IPAddress,
        status: server.status as StatusType
      }));
      
      setAccessibleServers(formattedServers);
    } catch (error: any) {
      console.error("Could not fetch servers:", error);
      toast.error("Failed to load servers");
      
      // Fallback to mock data if API fails
      setAccessibleServers([
        { 
          serverID: "server001", 
          name: "Database Server", 
          ipAddress: "192.168.1.10", 
          status: "online" as StatusType
        },
        { 
          serverID: "server002", 
          name: "Application Server", 
          ipAddress: "192.168.1.20", 
          status: "maintenance" as StatusType
        },
        { 
          serverID: "server003", 
          name: "Storage Server", 
          ipAddress: "192.168.1.30", 
          status: "online" as StatusType
        },
      ]);
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="user" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <section className="bg-blue-50/80 p-8 rounded-lg shadow-sm">
              <h1 className="text-3xl font-semibold mb-6">My Servers</h1>
              
              <p className="text-muted-foreground mb-6">
                View all the servers you have access to
              </p>
              
              <h2 className="text-2xl font-semibold mb-4">Accessible Servers</h2>
              
              {isLoading ? (
                <div className="flex justify-center items-center p-8">
                  <Spinner />
                </div>
              ) : (
                <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                  <table className="w-full">
                    <thead>
                      <tr className="bg-muted/50">
                        <th className="p-4 text-left">Server Name</th>
                        <th className="p-4 text-left">Server IP Address</th>
                        <th className="p-4 text-left">Status</th>
                      </tr>
                    </thead>
                    <tbody>
                      {accessibleServers.length > 0 ? (
                        accessibleServers.map((server) => (
                          <tr key={server.serverID} className="hover:bg-muted/30 transition-colors">
                            <td className="p-4 border-t">{server.name}</td>
                            <td className="p-4 border-t">{server.ipAddress}</td>
                            <td className="p-4 border-t">
                              <StatusBadge status={server.status} />
                            </td>
                          </tr>
                        ))
                      ) : (
                        <tr>
                          <td colSpan={3} className="text-center py-4 text-muted-foreground">
                            No accessible servers
                          </td>
                        </tr>
                      )}
                    </tbody>
                  </table>
                </div>
              )}
            </section>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Servers;
