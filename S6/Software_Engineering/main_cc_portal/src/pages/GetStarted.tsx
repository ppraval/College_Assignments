
import React from "react";
import { useNavigate } from "react-router-dom";
import { Button } from "@/components/ui/button";
import { motion } from "framer-motion";
import { Rocket, Server, Shield, Users, Database } from "lucide-react";

const GetStarted = () => {
  const navigate = useNavigate();

  const handleGetStarted = () => {
    navigate("/login");
  };

  return (
    <div className="min-h-screen overflow-hidden bg-gradient-to-br from-blue-50 to-indigo-50">
      {/* Hero Section */}
      <div className="container mx-auto px-6 py-16 text-center md:py-32">
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.5 }}
          className="mb-8"
        >
          <h1 className="mb-4 text-5xl font-bold tracking-tight text-gray-900 md:text-6xl lg:text-7xl">
            Command Center{" "}
            <span className="bg-gradient-to-r from-blue-600 to-indigo-600 bg-clip-text text-transparent">
              Portal
            </span>
          </h1>
          <p className="mx-auto mt-6 max-w-2xl text-xl text-gray-600">
            Your centralized platform for server management, monitoring, and resource allocation
          </p>
        </motion.div>

        <motion.div
          initial={{ opacity: 0, scale: 0.8 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ delay: 0.2, duration: 0.5 }}
        >
          <Button
            onClick={handleGetStarted}
            size="lg"
            className="animate-pulse rounded-full bg-gradient-to-r from-blue-600 to-indigo-600 px-8 py-6 text-lg hover:shadow-lg"
          >
            <Rocket className="mr-2" />
            Get Started
          </Button>
        </motion.div>

        {/* Feature highlights */}
        <motion.div
          initial={{ opacity: 0, y: 40 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ delay: 0.4, duration: 0.6 }}
          className="mt-20 grid grid-cols-1 gap-8 md:grid-cols-2 lg:grid-cols-4"
        >
          <FeatureCard
            icon={<Server />}
            title="Server Management"
            description="Centralized control of all server resources with real-time monitoring"
          />
          <FeatureCard
            icon={<Shield />}
            title="Secure Access"
            description="Role-based permissions for administrators and regular users"
          />
          <FeatureCard
            icon={<Database />}
            title="Resource Allocation"
            description="Efficient distribution and tracking of computing resources"
          />
          <FeatureCard
            icon={<Users />}
            title="Team Collaboration"
            description="Seamless communication and issue tracking between team members"
          />
        </motion.div>
      </div>

      {/* Decorative elements */}
      <div className="absolute bottom-0 left-0 right-0 -z-10 h-1/3 bg-gradient-to-t from-blue-100 to-transparent opacity-70"></div>
      <div className="absolute -top-24 -right-24 -z-10 h-96 w-96 rounded-full bg-blue-200 opacity-20 blur-3xl"></div>
      <div className="absolute -bottom-24 -left-24 -z-10 h-96 w-96 rounded-full bg-indigo-200 opacity-20 blur-3xl"></div>
    </div>
  );
};

// Feature card component
const FeatureCard = ({ icon, title, description }: { icon: React.ReactNode, title: string, description: string }) => (
  <motion.div
    whileHover={{ y: -5, transition: { duration: 0.2 } }}
    className="rounded-xl bg-white p-6 shadow-md transition-all hover:shadow-lg"
  >
    <div className="mb-4 inline-flex rounded-full bg-blue-100 p-3 text-blue-600">
      {icon}
    </div>
    <h3 className="mb-2 text-xl font-semibold">{title}</h3>
    <p className="text-gray-600">{description}</p>
  </motion.div>
);

export default GetStarted;
