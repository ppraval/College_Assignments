
import { useState, useEffect } from "react";
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Textarea } from "@/components/ui/textarea";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Label } from "@/components/ui/label";
import { StatusBadge, StatusType } from "@/components/StatusBadge";
import { toast } from "sonner";
import { Spinner } from "@/components/ui/spinner";

const API_BASE_URL = "http://localhost:5000";

interface Issue {
  id: number;
  title: string;
  description: string;
  priority: string;
  status: StatusType;
  created_at: string;
}

const Issues = () => {
  const [issueTitle, setIssueTitle] = useState("");
  const [issueDescription, setIssueDescription] = useState("");
  const [issuePriority, setIssuePriority] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [reportedIssues, setReportedIssues] = useState<Issue[]>([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetchUserIssues();
  }, []);

  const fetchUserIssues = async () => {
    try {
      setLoading(true);
      const response = await fetch(`${API_BASE_URL}/api/issues`);
      
      if (!response.ok) {
        throw new Error(`Failed to fetch issues: ${response.statusText}`);
      }
      
      const data = await response.json();
      
      // Filter issues for the current user (in a real app, this would be done by the server)
      // For now, we'll just use all issues for demonstration
      setReportedIssues(data);
    } catch (error: any) {
      console.error("Error fetching issues:", error);
      toast.error("Failed to load your issues");
      
      // Fallback to mock data
      setReportedIssues([
        { 
          id: 1, 
          title: "Wi-Fi Not Working", 
          description: "Unable to connect to campus Wi-Fi", 
          priority: "High", 
          status: "open" as StatusType,
          created_at: new Date().toISOString()
        },
        { 
          id: 2, 
          title: "Server Downtime", 
          description: "The research server is down affecting project work", 
          priority: "Medium", 
          status: "solved" as StatusType,
          created_at: new Date().toISOString()
        },
      ]);
    } finally {
      setLoading(false);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!issueTitle || !issueDescription || !issuePriority) {
      toast.error("Please fill in all fields");
      return;
    }
    
    setIsSubmitting(true);
    
    try {
      // Prepare the issue data
      const issueData = {
        title: issueTitle,
        description: issueDescription,
        priority: issuePriority,
        // In a real app, user_id would be taken from authentication
        user_id: "student001" // Default user ID for testing
      };
      
      console.log("Submitting issue:", issueData);
      
      // Send the issue to the server
      const response = await fetch(`${API_BASE_URL}/api/issues`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(issueData),
      });
      
      if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to submit issue: ${response.statusText}. ${errorText ? `Server response: ${errorText}` : ''}`);
      }
      
      toast.success("Issue reported successfully");
      
      // Reset form
      setIssueTitle("");
      setIssueDescription("");
      setIssuePriority("");
      
      // Refresh issues list
      fetchUserIssues();
    } catch (error: any) {
      console.error("Error submitting issue:", error);
      toast.error(error.message || "Failed to submit issue");
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="user" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <section className="mb-12">
              <h1 className="text-3xl font-semibold mb-6">Report an Issue</h1>
              
              <div className="bg-white rounded-lg shadow-sm border border-border p-6">
                <p className="text-sm text-muted-foreground mb-6">
                  Submit a new Issue
                </p>
                
                <form onSubmit={handleSubmit} className="space-y-4">
                  <div className="space-y-2">
                    <Label htmlFor="issue-title">Issue Title</Label>
                    <Input
                      id="issue-title"
                      placeholder="Enter Issue Title"
                      value={issueTitle}
                      onChange={(e) => setIssueTitle(e.target.value)}
                      required
                    />
                  </div>
                  
                  <div className="space-y-2">
                    <Label htmlFor="issue-description">Issue Description</Label>
                    <Textarea
                      id="issue-description"
                      placeholder="Enter Issue Description (Details)"
                      value={issueDescription}
                      onChange={(e) => setIssueDescription(e.target.value)}
                      rows={4}
                      required
                    />
                  </div>
                  
                  <div className="space-y-2">
                    <Label htmlFor="issue-priority">Priority</Label>
                    <Select
                      value={issuePriority}
                      onValueChange={setIssuePriority}
                    >
                      <SelectTrigger id="issue-priority">
                        <SelectValue placeholder="Select Priority" />
                      </SelectTrigger>
                      <SelectContent>
                        <SelectItem value="High">High</SelectItem>
                        <SelectItem value="Medium">Medium</SelectItem>
                        <SelectItem value="Low">Low</SelectItem>
                      </SelectContent>
                    </Select>
                  </div>
                  
                  <div className="pt-2">
                    <Button type="submit" className="w-full sm:w-auto" disabled={isSubmitting}>
                      {isSubmitting ? <><Spinner className="mr-2" size="sm"/> Submitting...</> : "Submit"}
                    </Button>
                  </div>
                </form>
              </div>
            </section>
            
            <section>
              <div className="flex items-center justify-between mb-4">
                <h2 className="text-2xl font-semibold">My Reported Issues</h2>
                <Button 
                  variant="outline" 
                  onClick={fetchUserIssues}
                  disabled={loading}
                >
                  {loading ? <Spinner className="mr-2" size="sm" /> : "Refresh"}
                </Button>
              </div>
              
              {loading ? (
                <div className="flex justify-center py-8">
                  <Spinner />
                </div>
              ) : (
                <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                  <table className="w-full">
                    <thead>
                      <tr className="bg-muted/50">
                        <th className="px-4 py-2 text-left">Title</th>
                        <th className="px-4 py-2 text-left">Description</th>
                        <th className="px-4 py-2 text-left">Priority</th>
                        <th className="px-4 py-2 text-left">Status</th>
                      </tr>
                    </thead>
                    <tbody>
                      {reportedIssues.length > 0 ? (
                        reportedIssues.map((issue) => (
                          <tr key={issue.id} className="hover:bg-muted/30 transition-colors">
                            <td className="px-4 py-3 border-t">{issue.title}</td>
                            <td className="px-4 py-3 border-t">{issue.description}</td>
                            <td className="px-4 py-3 border-t">
                              <StatusBadge status={issue.priority.toLowerCase() as "high" | "medium" | "low"}>
                                {issue.priority}
                              </StatusBadge>
                            </td>
                            <td className="px-4 py-3 border-t">
                              <StatusBadge status={issue.status} />
                            </td>
                          </tr>
                        ))
                      ) : (
                        <tr>
                          <td colSpan={4} className="text-center py-4 text-muted-foreground">
                            No reported issues
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

export default Issues;
