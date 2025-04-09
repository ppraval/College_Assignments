
import { useState, useEffect } from "react";
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { Button } from "@/components/ui/button";
import { StatusBadge } from "@/components/StatusBadge";
import { toast } from "sonner";
import { Spinner } from "@/components/Spinner";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";

interface Issue {
  id: number;
  user: string;
  title: string;
  description: string;
  priority: "High" | "Medium" | "Low";
  status: "pending" | "resolved";
}

const Issues = () => {
  const [issues, setIssues] = useState<Issue[]>([]);
  const [loading, setLoading] = useState(true);
  const apiUrl = import.meta.env.VITE_API_URL || 'http://localhost:5000/api';

  useEffect(() => {
    fetchIssues();
  }, []);

  const fetchIssues = async () => {
    try {
      setLoading(true);
      const response = await fetch(`${apiUrl}/issues`);
      
      if (!response.ok) {
        throw new Error("Failed to fetch issues");
      }
      
      const data = await response.json();
      
      // If no issues are returned, use mock data for demonstration
      if (data.length === 0) {
        setIssues([
          { 
            id: 1, 
            user: "John Doe", 
            title: "Wi-Fi not working", 
            description: "Cannot connect to Campus Wi-Fi", 
            priority: "High",
            status: "pending"
          },
          { 
            id: 2, 
            user: "Jane Smith", 
            title: "VPN Issue", 
            description: "Unable to connect to VPN from home", 
            priority: "Medium",
            status: "pending"
          },
          { 
            id: 3, 
            user: "Alex Johnson", 
            title: "Email Problems", 
            description: "Cannot send or receive emails since this morning", 
            priority: "High",
            status: "pending"
          },
        ]);
      } else {
        // Transform the API response to match our interface
        const formattedIssues = data.map((issue: any) => ({
          id: issue.id || issue.issueID,
          user: issue.username || issue.user,
          title: issue.title || issue.type,
          description: issue.description,
          priority: issue.priority || "Medium",
          status: issue.status || "pending"
        }));
        setIssues(formattedIssues);
      }
    } catch (error) {
      console.error("Error fetching issues:", error);
      toast.error("Failed to load issues");
      
      // Fallback to mock data
      setIssues([
        { 
          id: 1, 
          user: "John Doe", 
          title: "Wi-Fi not working", 
          description: "Cannot connect to Campus Wi-Fi", 
          priority: "High",
          status: "pending"
        },
        { 
          id: 2, 
          user: "Jane Smith", 
          title: "VPN Issue", 
          description: "Unable to connect to VPN from home", 
          priority: "Medium",
          status: "pending"
        },
        { 
          id: 3, 
          user: "Alex Johnson", 
          title: "Email Problems", 
          description: "Cannot send or receive emails since this morning", 
          priority: "High",
          status: "pending"
        },
      ]);
    } finally {
      setLoading(false);
    }
  };

  const handleResolve = async (id: number) => {
    try {
      const response = await fetch(`${apiUrl}/issues/${id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ status: 'resolved' }),
      });

      if (!response.ok) {
        throw new Error("Failed to update issue status");
      }

      // Update local state
      setIssues(prevIssues => 
        prevIssues.map(issue => 
          issue.id === id ? { ...issue, status: 'resolved' } : issue
        )
      );
      
      toast.success(`Issue #${id} has been marked as resolved`);
    } catch (error) {
      console.error("Error updating issue:", error);
      toast.error("Failed to resolve issue");
      
      // Update local state anyway for demo purposes
      setIssues(prevIssues => 
        prevIssues.map(issue => 
          issue.id === id ? { ...issue, status: 'resolved' } : issue
        )
      );
    }
  };

  const pendingIssues = issues.filter(issue => issue.status !== "resolved");

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="admin" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <h1 className="text-3xl font-semibold mb-6">Manage Issues</h1>
            
            <p className="text-muted-foreground mb-6">
              View Issues reported by the users
            </p>
            
            <section>
              <h2 className="text-2xl font-semibold mb-4">Pending Issues</h2>
              
              <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                {loading ? (
                  <div className="flex justify-center items-center p-8">
                    <Spinner />
                  </div>
                ) : (
                  <Table>
                    <TableHeader>
                      <TableRow>
                        <TableHead>User</TableHead>
                        <TableHead>Issue Title</TableHead>
                        <TableHead>Description</TableHead>
                        <TableHead>Priority</TableHead>
                        <TableHead>Action</TableHead>
                      </TableRow>
                    </TableHeader>
                    <TableBody>
                      {pendingIssues.length > 0 ? (
                        pendingIssues.map((issue) => (
                          <TableRow key={issue.id} className="hover:bg-muted/30 transition-colors">
                            <TableCell>{issue.user}</TableCell>
                            <TableCell>{issue.title}</TableCell>
                            <TableCell>{issue.description}</TableCell>
                            <TableCell>
                              <StatusBadge 
                                status={issue.priority.toLowerCase() as "high" | "medium" | "low"}
                              >
                                {issue.priority}
                              </StatusBadge>
                            </TableCell>
                            <TableCell>
                              <Button 
                                onClick={() => handleResolve(issue.id)}
                                className="bg-green-500 hover:bg-green-600 text-white"
                                size="sm"
                              >
                                Mark as Resolved
                              </Button>
                            </TableCell>
                          </TableRow>
                        ))
                      ) : (
                        <TableRow>
                          <TableCell colSpan={5} className="text-center py-4 text-muted-foreground">
                            No pending issues
                          </TableCell>
                        </TableRow>
                      )}
                    </TableBody>
                  </Table>
                )}
              </div>
            </section>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Issues;
