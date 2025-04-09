
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { StatCard } from "@/components/StatCard";

const Dashboard = () => {
  // Mock data for Admin Dashboard
  const stats = {
    totalUsers: 1250,
    pendingRequests: 15
  };

  const recentUpdates = [
    { id: 1, description: "Praval Pattam updated their profile", timestamp: "1 hour ago" },
    { id: 2, description: "New Request submitted by Abhishek", timestamp: "3 hours ago" },
    { id: 3, description: "System Maintenance scheduled for Friday", timestamp: "5 hours ago" }
  ];

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="admin" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <h1 className="text-3xl font-semibold mb-6">Welcome, Admin</h1>
            
            <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mb-10">
              <StatCard title="Total Users" value={stats.totalUsers} />
              <StatCard title="Pending Requests" value={stats.pendingRequests} />
            </div>
            
            <section className="mb-6">
              <h2 className="text-2xl font-semibold mb-4">Recent Updates</h2>
              <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                <ul className="divide-y divide-border">
                  {recentUpdates.map((update) => (
                    <li key={update.id} className="p-4 hover:bg-muted/50 transition-colors">
                      <div className="flex items-start justify-between">
                        <p className="text-sm">{update.description}</p>
                        <span className="text-xs text-muted-foreground">{update.timestamp}</span>
                      </div>
                    </li>
                  ))}
                </ul>
              </div>
            </section>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Dashboard;
