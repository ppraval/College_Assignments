
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { StatCard } from "@/components/StatCard";

const Dashboard = () => {
  // Mock data for User Dashboard
  const stats = {
    totalRequests: 12,
    approvedRequests: 9,
    pendingRequests: 3,
    issuesRaised: 5
  };

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="user" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <h1 className="text-3xl font-semibold mb-6">Welcome, User</h1>
            
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6 mb-10">
              <StatCard title="Total Requests" value={stats.totalRequests} />
              <StatCard title="Approved Requests" value={stats.approvedRequests} />
              <StatCard title="Pending Requests" value={stats.pendingRequests} />
            </div>
            
            <div className="grid grid-cols-1 gap-6">
              <StatCard title="Issues Raised" value={stats.issuesRaised} />
            </div>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Dashboard;
