
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
import { Alert, AlertDescription } from "@/components/ui/alert";
import { AlertCircle } from "lucide-react";

// Define the base URL for API calls
const API_BASE_URL = "http://localhost:5000";

interface Request {
  requestID: string;
  type: string;
  description: string;
  status: StatusType;
  timestamp: string;
  userName: string;
  serverName: string | null;
  userID: string;
  serverID: string | null;
}

const Requests = () => {
  const [requestType, setRequestType] = useState("");
  const [requestDescription, setRequestDescription] = useState("");
  const [requestPriority, setRequestPriority] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [userRequests, setUserRequests] = useState<Request[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [debugInfo, setDebugInfo] = useState<string | null>(null);

  // Fetch user's requests on component mount
  useEffect(() => {
    fetchUserRequests();
  }, []);

  const fetchUserRequests = async () => {
    try {
      setLoading(true);
      const token = localStorage.getItem("token");
      
      if (!token) {
        throw new Error("No authentication token found");
      }
      
      console.log("Fetching user requests from:", `${API_BASE_URL}/api/requests`);
      const response = await fetch(`${API_BASE_URL}/api/requests`, {
        headers: {
          Authorization: `Bearer ${token}`,
        },
      });
      
      if (!response.ok) {
        const errorText = await response.text();
        console.error("Response not OK:", response.status, errorText);
        throw new Error(`Failed to fetch requests: ${response.statusText}. Server response: ${errorText.substring(0, 100)}...`);
      }
      
      const data = await response.json();
      console.log("User requests data received:", data);
      
      if (Array.isArray(data)) {
        setUserRequests(data);
        setDebugInfo(data.length === 0 ? "Server returned an empty array. Check database tables." : null);
      } else {
        setUserRequests([]);
        setDebugInfo(`Server returned non-array data: ${JSON.stringify(data)}`);
      }
      
      setError(null);
    } catch (err: any) {
      console.error("Error fetching user requests:", err);
      setError(err.message || "Failed to fetch requests");
      setDebugInfo(`Error details: ${err}`);
      toast.error("Failed to load your requests");
    } finally {
      setLoading(false);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!requestType || !requestDescription || !requestPriority) {
      toast.error("Please fill in all fields");
      return;
    }
    
    setIsSubmitting(true);
    
    try {
      const token = localStorage.getItem("token");
      
      if (!token) {
        throw new Error("No authentication token found");
      }
      
      // Prepare the request data with correct field names
      const requestData = {
        type: requestType,
        description: requestDescription,
        priority: requestPriority
      };
      
      console.log("Submitting request:", requestData);
      
      // Send the request to the server
      const response = await fetch(`${API_BASE_URL}/api/requests`, {
        method: "POST",
        headers: {
          Authorization: `Bearer ${token}`,
          "Content-Type": "application/json",
        },
        body: JSON.stringify(requestData),
      });
      
      if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to submit request: ${response.statusText}. Server response: ${errorText.substring(0, 100)}...`);
      }
      
      const responseData = await response.json();
      console.log("Server response:", responseData);
      
      toast.success("Request submitted successfully");
      
      // Reset form
      setRequestType("");
      setRequestDescription("");
      setRequestPriority("");
      
      // Refresh user's requests
      fetchUserRequests();
    } catch (err: any) {
      console.error("Error submitting request:", err);
      toast.error(err.message || "Failed to submit request");
    } finally {
      setIsSubmitting(false);
    }
  };

  // Format date function
  const formatDate = (dateString: string) => {
    try {
      const date = new Date(dateString);
      return new Intl.DateTimeFormat('en-US', {
        dateStyle: 'medium',
        timeStyle: 'short'
      }).format(date);
    } catch (err) {
      console.error("Date formatting error:", err);
      return dateString || "Unknown date";
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
              <h1 className="text-3xl font-semibold mb-6">Raise a Request</h1>
              
              <div className="bg-white rounded-lg shadow-sm border border-border p-6">
                <p className="text-sm text-muted-foreground mb-6">
                  Submit a new Request
                </p>
                
                <form onSubmit={handleSubmit} className="space-y-4">
                  <div className="space-y-2">
                    <Label htmlFor="request-title">Request Title</Label>
                    <Input
                      id="request-title"
                      placeholder="Enter Request Title"
                      value={requestType}
                      onChange={(e) => setRequestType(e.target.value)}
                      required
                    />
                  </div>
                  
                  <div className="space-y-2">
                    <Label htmlFor="request-description">Request Description</Label>
                    <Textarea
                      id="request-description"
                      placeholder="Enter Request Description (Details)"
                      value={requestDescription}
                      onChange={(e) => setRequestDescription(e.target.value)}
                      rows={4}
                      required
                    />
                  </div>
                  
                  <div className="space-y-2">
                    <Label htmlFor="request-priority">Priority</Label>
                    <Select
                      value={requestPriority}
                      onValueChange={setRequestPriority}
                    >
                      <SelectTrigger id="request-priority">
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
                      {isSubmitting ? <><Spinner className="mr-2" /> Submitting...</> : "Submit"}
                    </Button>
                  </div>
                </form>
              </div>
            </section>
            
            <section>
              <div className="flex justify-between items-center mb-4">
                <h2 className="text-2xl font-semibold">My Previous Requests</h2>
                <Button 
                  variant="outline" 
                  onClick={fetchUserRequests}
                  disabled={loading}
                >
                  {loading ? <Spinner className="mr-2" /> : null}
                  Refresh
                </Button>
              </div>
              
              {error && (
                <Alert variant="destructive" className="mb-6">
                  <AlertCircle className="h-4 w-4" />
                  <AlertDescription>{error}</AlertDescription>
                </Alert>
              )}
              
              {debugInfo && (
                <Alert className="mb-6 bg-yellow-50 border-yellow-200">
                  <AlertCircle className="h-4 w-4 text-yellow-500" />
                  <AlertDescription className="text-yellow-700">
                    Debug info: {debugInfo}
                  </AlertDescription>
                </Alert>
              )}
              
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
                        <th className="px-4 py-2 text-left">Date</th>
                        <th className="px-4 py-2 text-left">Status</th>
                      </tr>
                    </thead>
                    <tbody>
                      {userRequests.length > 0 ? (
                        userRequests.map((request) => (
                          <tr key={request.requestID} className="hover:bg-muted/30 transition-colors">
                            <td className="px-4 py-3 border-t">{request.type}</td>
                            <td className="px-4 py-3 border-t">{request.description}</td>
                            <td className="px-4 py-3 border-t">
                              <StatusBadge 
                                status={request.type.toLowerCase() as "high" | "medium" | "low"}
                              >
                                {request.type}
                              </StatusBadge>
                            </td>
                            <td className="px-4 py-3 border-t">{formatDate(request.timestamp)}</td>
                            <td className="px-4 py-3 border-t">
                              <StatusBadge status={request.status} />
                            </td>
                          </tr>
                        ))
                      ) : (
                        <tr>
                          <td colSpan={5} className="text-center py-4 text-muted-foreground">
                            No previous requests found. Try refreshing or creating a new request.
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

export default Requests;
