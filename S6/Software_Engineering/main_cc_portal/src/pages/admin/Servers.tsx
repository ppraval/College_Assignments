
import React, { useState, useEffect } from 'react';
import { Header } from "@/components/Header";
import { Sidebar } from "@/components/Sidebar";
import { PageTransition } from "@/components/PageTransition";
import {
  Table,
  TableBody,
  TableCaption,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table"
import { Button } from "@/components/ui/button"
import { Dialog, DialogContent, DialogDescription, DialogFooter, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog"
import { Input } from "@/components/ui/input"
import { Label } from "@/components/ui/label"
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select"
import { toast } from "sonner"
import { Spinner } from "@/components/ui/spinner"

interface Server {
  serverID: string;
  name: string;
  ipAddress: string;
  status: string;
}

const AddServerModal = ({
  isOpen,
  onClose,
  onAddServer
}: {
  isOpen: boolean;
  onClose: () => void;
  onAddServer: (serverData: any) => void;
}) => {
  const [name, setName] = useState('');
  const [ip, setIp] = useState('');
  const [status, setStatus] = useState('online');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setIsSubmitting(true);
    
    // Only send fields that exist in the database schema
    const serverData = {
      name,
      ip,
      status
    };
    
    onAddServer(serverData);
    setIsSubmitting(false);
    resetForm();
  };

  const resetForm = () => {
    setName('');
    setIp('');
    setStatus('online');
  };

  return (
    <Dialog open={isOpen} onOpenChange={onClose}>
      <DialogContent className="sm:max-w-[425px]">
        <DialogHeader>
          <DialogTitle>Add New Server</DialogTitle>
          <DialogDescription>
            Enter the details for the new server.
          </DialogDescription>
        </DialogHeader>
        <form onSubmit={handleSubmit}>
          <div className="grid gap-4 py-4">
            <div className="grid grid-cols-4 items-center gap-4">
              <Label htmlFor="name" className="text-right">
                Name
              </Label>
              <Input
                id="name"
                value={name}
                onChange={(e) => setName(e.target.value)}
                className="col-span-3"
                required
              />
            </div>
            <div className="grid grid-cols-4 items-center gap-4">
              <Label htmlFor="ip" className="text-right">
                IP Address
              </Label>
              <Input
                id="ip"
                value={ip}
                onChange={(e) => setIp(e.target.value)}
                className="col-span-3"
                required
                placeholder="192.168.1.1"
              />
            </div>
            <div className="grid grid-cols-4 items-center gap-4">
              <Label htmlFor="status" className="text-right">
                Status
              </Label>
              <Select value={status} onValueChange={setStatus}>
                <SelectTrigger className="col-span-3">
                  <SelectValue placeholder="Select status" />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="online">Online</SelectItem>
                  <SelectItem value="offline">Offline</SelectItem>
                  <SelectItem value="maintenance">Maintenance</SelectItem>
                </SelectContent>
              </Select>
            </div>
          </div>
          <DialogFooter>
            <Button type="button" variant="outline" onClick={onClose}>
              Cancel
            </Button>
            <Button type="submit" disabled={isSubmitting}>
              {isSubmitting ? <Spinner size="sm" /> : 'Add Server'}
            </Button>
          </DialogFooter>
        </form>
      </DialogContent>
    </Dialog>
  );
};

const Servers = () => {
  const [servers, setServers] = useState<Server[]>([]);
  const [isAddServerModalOpen, setIsAddServerModalOpen] = useState(false);
  const [isLoading, setIsLoading] = useState(false);

  useEffect(() => {
    fetchServers();
  }, []);

  const fetchServers = async () => {
    setIsLoading(true);
    try {
      const response = await fetch('http://localhost:5000/api/servers');
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      
      // Transform the data to match our Server interface
      const formattedServers: Server[] = data.map((server: any) => ({
        serverID: server.serverID,
        name: server.name,
        ipAddress: server.IPAddress, // Map IPAddress from API to ipAddress in our interface
        status: server.status
      }));
      
      setServers(formattedServers);
    } catch (error: any) {
      toast.error("Error fetching servers", {
        description: error.message
      });
      console.error("Could not fetch servers:", error);
    } finally {
      setIsLoading(false);
    }
  };

  const handleAddServer = async (serverData: any) => {
    try {
      const response = await fetch('http://localhost:5000/api/servers', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(serverData),
      });

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }

      toast.success("Server added successfully");
      fetchServers(); // Refresh the server list
    } catch (error: any) {
      toast.error("Error adding server", {
        description: error.message
      });
      console.error("Could not add server:", error);
    } finally {
      setIsAddServerModalOpen(false);
    }
  };

  return (
    <div className="min-h-screen bg-background">
      <Header />
      <Sidebar role="admin" />
      
      <PageTransition>
        <main className="pt-24 pl-72 pr-8 pb-16 animate-fade-in">
          <div className="max-w-6xl">
            <section className="bg-white p-8 rounded-lg shadow-sm border border-border">
              <div className="flex items-center justify-between mb-6">
                <h1 className="text-2xl font-bold">Servers</h1>
                <Button onClick={() => setIsAddServerModalOpen(true)}>
                  Add Server
                </Button>
              </div>

              <AddServerModal
                isOpen={isAddServerModalOpen}
                onClose={() => setIsAddServerModalOpen(false)}
                onAddServer={handleAddServer}
              />

              <div className="relative overflow-x-auto">
                {isLoading ? (
                  <div className="absolute inset-0 bg-gray-100 dark:bg-gray-700 opacity-50 flex items-center justify-center">
                    <Spinner />
                  </div>
                ) : null}
                <Table>
                  <TableCaption>A list of your recent servers.</TableCaption>
                  <TableHeader>
                    <TableRow>
                      <TableHead className="w-[100px]">ID</TableHead>
                      <TableHead>Name</TableHead>
                      <TableHead>IP Address</TableHead>
                      <TableHead>Status</TableHead>
                    </TableRow>
                  </TableHeader>
                  <TableBody>
                    {servers.map((server) => (
                      <TableRow key={server.serverID}>
                        <TableCell className="font-medium">{server.serverID}</TableCell>
                        <TableCell>{server.name}</TableCell>
                        <TableCell>{server.ipAddress}</TableCell>
                        <TableCell>{server.status}</TableCell>
                      </TableRow>
                    ))}
                  </TableBody>
                </Table>
              </div>
            </section>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default Servers;
