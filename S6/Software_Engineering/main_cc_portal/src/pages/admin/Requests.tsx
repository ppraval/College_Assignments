
import { useState, useEffect } from "react";
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import { Button } from "@/components/ui/button";
import { StatusBadge } from "@/components/StatusBadge";
import { toast } from "sonner";
import { Check, X, RefreshCw } from "lucide-react";
import { Spinner } from "@/components/Spinner";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";
import { Alert, AlertDescription } from "@/components/ui/alert";
import { useNavigate } from "react-router-dom";
import { useAuth } from "@/context/AuthContext";

interface Request {
  requestID: string;
  type: string;
  description: string;
  status: "pending" | "approved" | "rejected";
  timestamp: string;
  userName: string;
  serverName: string | null;
  userID: string;
  serverID: string | null;
}

const API_BASE_URL = "http://localhost:5000"; // Define the base URL for API calls

const Requests = () => {
  const [requests, setRequests] = useState<Request[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [pendingRequests, setPendingRequests] = useState<Request[]>([]);
  const navigate = useNavigate();
  const { logout, userID, userName, userRole } = useAuth();

  const fetchRequests = async () => {
    try {
      setLoading(true);
      setError(null);
      
      console.log("Fetching requests from:", `${API_BASE_URL}/api/requests`);
      
      // Simplified fetch without authentication header
      const response = await fetch(`${API_BASE_URL}/api/requests`);
      
      if (!response.ok) {
        const errorText = await response.text();
        console.error("Response error:", response.status, errorText);
        throw new Error(`Failed to fetch requests: ${response.statusText}. ${errorText ? `Server response: ${errorText}` : ''}`);
      }
      
      const data = await response.json();
      console.log("Requests data received:", data);
      
      if (!Array.isArray(data)) {
        console.error("Received non-array data:", data);
        throw new Error("Server returned invalid data format");
      }
      
      if (data.length === 0) {
        console.log("No requests found in the response");
        // We'll still set the empty array but won't show an error
      }
      
      setRequests(data);
      
      // Filter for pending requests
      const pending = data.filter((req: Request) => req.status === "pending");
      setPendingRequests(pending);
      
    } catch (err: any) {
      console.error("Error fetching requests:", err);
      setError(err.message || "Failed to fetch requests");
      toast.error("Failed to load requests");
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchRequests();
    
    // Set up an interval to refresh data every 30 seconds
    const intervalId = setInterval(fetchRequests, 30000);
    
    // Clean up interval on component unmount
    return () => clearInterval(intervalId);
  }, []);

  const handleApprove = async (id: string) => {
    try {
      // Simplified fetch without authentication header
      const response = await fetch(`${API_BASE_URL}/api/requests/${id}`, {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ status: "approved" }),
      });
      
      if (!response.ok) {
        throw new Error(`Failed to approve request: ${response.statusText}`);
      }
      
      // Update the local state
      setRequests(requests.map(req => 
        req.requestID === id ? { ...req, status: "approved" } : req
      ));
      
      // Update pending requests
      setPendingRequests(pendingRequests.filter(req => req.requestID !== id));
      
      toast.success(`Request #${id} has been approved`);
    } catch (err: any) {
      toast.error(err.message || "Failed to approve request");
    }
  };

  const handleReject = async (id: string) => {
    try {
      // Simplified fetch without authentication header
      const response = await fetch(`${API_BASE_URL}/api/requests/${id}`, {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ status: "rejected" }),
      });
      
      if (!response.ok) {
        throw new Error(`Failed to reject request: ${response.statusText}`);
      }
      
      // Update the local state
      setRequests(requests.map(req => 
        req.requestID === id ? { ...req, status: "rejected" } : req
      ));
      
      // Update pending requests
      setPendingRequests(pendingRequests.filter(req => req.requestID !== id));
      
      toast.error(`Request #${id} has been rejected`);
    } catch (err: any) {
      toast.error(err.message || "Failed to reject request");
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
      <Sidebar role="admin" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <div className="flex items-center justify-between mb-6">
              <h1 className="text-3xl font-semibold">Manage User Requests</h1>
              <Button 
                variant="outline" 
                onClick={fetchRequests}
                disabled={loading}
              >
                {loading ? <Spinner className="mr-2" /> : <RefreshCw className="mr-2 h-4 w-4" />}
                Refresh
              </Button>
            </div>
            
            <p className="text-muted-foreground mb-6">
              Approve or Reject requests based on details provided by the user
            </p>
            
            {error && (
              <Alert variant="destructive" className="mb-6">
                <AlertDescription>
                  {error}
                </AlertDescription>
              </Alert>
            )}
            
            <section>
              <h2 className="text-2xl font-semibold mb-4">Pending Requests</h2>
              
              {loading ? (
                <div className="flex justify-center py-8">
                  <Spinner />
                </div>
              ) : (
                <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                  <Table>
                    <TableHeader>
                      <TableRow>
                        <TableHead>User</TableHead>
                        <TableHead>Request Type</TableHead>
                        <TableHead>Description</TableHead>
                        <TableHead>Server</TableHead>
                        <TableHead>Date</TableHead>
                        <TableHead>Action</TableHead>
                      </TableRow>
                    </TableHeader>
                    <TableBody>
                      {pendingRequests.length > 0 ? (
                        pendingRequests.map((request) => (
                          <TableRow key={request.requestID}>
                            <TableCell>{request.userName}</TableCell>
                            <TableCell>
                              <StatusBadge 
                                status={request.type.toLowerCase() as "high" | "medium" | "low"}
                              >
                                {request.type}
                              </StatusBadge>
                            </TableCell>
                            <TableCell>{request.description}</TableCell>
                            <TableCell>{request.serverName || 'N/A'}</TableCell>
                            <TableCell>{formatDate(request.timestamp)}</TableCell>
                            <TableCell>
                              <div className="flex space-x-2">
                                <Button 
                                  onClick={() => handleApprove(request.requestID)}
                                  className="bg-green-500 hover:bg-green-600 text-white"
                                  size="sm"
                                >
                                  <Check className="mr-1 h-4 w-4" /> Approve
                                </Button>
                                <Button 
                                  onClick={() => handleReject(request.requestID)}
                                  className="bg-red-500 hover:bg-red-600 text-white"
                                  size="sm"
                                >
                                  <X className="mr-1 h-4 w-4" /> Reject
                                </Button>
                              </div>
                            </TableCell>
                          </TableRow>
                        ))
                      ) : (
                        <TableRow>
                          <TableCell colSpan={6} className="text-center py-8 text-muted-foreground">
                            <div className="flex flex-col items-center justify-center">
                              <p className="mb-2">No pending requests found</p>
                              <Button variant="outline" size="sm" onClick={fetchRequests}>
                                <RefreshCw className="mr-1 h-4 w-4" /> Refresh
                              </Button>
                            </div>
                          </TableCell>
                        </TableRow>
                      )}
                    </TableBody>
                  </Table>
                </div>
              )}
            </section>
            
            <section className="mt-12">
              <h2 className="text-2xl font-semibold mb-4">All Requests</h2>
              
              {loading ? (
                <div className="flex justify-center py-8">
                  <Spinner />
                </div>
              ) : (
                <div className="bg-white rounded-lg shadow-sm border border-border overflow-hidden">
                  <Table>
                    <TableHeader>
                      <TableRow>
                        <TableHead>User</TableHead>
                        <TableHead>Request Type</TableHead>
                        <TableHead>Description</TableHead>
                        <TableHead>Server</TableHead>
                        <TableHead>Date</TableHead>
                        <TableHead>Status</TableHead>
                      </TableRow>
                    </TableHeader>
                    <TableBody>
                      {requests.length > 0 ? (
                        requests.map((request) => (
                          <TableRow key={request.requestID}>
                            <TableCell>{request.userName}</TableCell>
                            <TableCell>
                              <StatusBadge 
                                status={request.type.toLowerCase() as "high" | "medium" | "low"}
                              >
                                {request.type}
                              </StatusBadge>
                            </TableCell>
                            <TableCell>{request.description}</TableCell>
                            <TableCell>{request.serverName || 'N/A'}</TableCell>
                            <TableCell>{formatDate(request.timestamp)}</TableCell>
                            <TableCell>
                              <StatusBadge status={request.status}>
                                {request.status.charAt(0).toUpperCase() + request.status.slice(1)}
                              </StatusBadge>
                            </TableCell>
                          </TableRow>
                        ))
                      ) : (
                        <TableRow>
                          <TableCell colSpan={6} className="text-center py-8 text-muted-foreground">
                            <div className="flex flex-col items-center justify-center">
                              <p className="mb-2">No requests found</p>
                              <Button variant="outline" size="sm" onClick={fetchRequests}>
                                <RefreshCw className="mr-1 h-4 w-4" /> Refresh
                              </Button>
                            </div>
                          </TableCell>
                        </TableRow>
                      )}
                    </TableBody>
                  </Table>
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
