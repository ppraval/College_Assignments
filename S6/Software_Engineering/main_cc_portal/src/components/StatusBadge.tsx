
import { cn } from "@/lib/utils";

export type StatusType = 
  | "pending" 
  | "approved" 
  | "rejected" 
  | "open" 
  | "solved" 
  | "maintenance" 
  | "online"
  | "high"
  | "medium"
  | "low";

interface StatusBadgeProps {
  status: StatusType;
  className?: string;
  children?: React.ReactNode;
}

export function StatusBadge({ status, className, children }: StatusBadgeProps) {
  const getStatusClasses = () => {
    switch (status) {
      case "pending":
        return "bg-yellow-100 text-yellow-800";
      case "approved":
        return "bg-green-100 text-green-800";
      case "rejected":
        return "bg-red-100 text-red-800";
      case "open":
        return "bg-yellow-100 text-yellow-800";
      case "solved":
        return "bg-green-100 text-green-800";
      case "maintenance":
        return "bg-indigo-100 text-indigo-800";
      case "online":
        return "bg-green-100 text-green-800";
      case "high":
        return "bg-red-100 text-red-800";
      case "medium":
        return "bg-yellow-100 text-yellow-800";
      case "low":
        return "bg-blue-100 text-blue-800";
      default:
        return "bg-gray-100 text-gray-800";
    }
  };

  return (
    <span
      className={cn(
        "inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium",
        getStatusClasses(),
        className
      )}
    >
      {children || status.charAt(0).toUpperCase() + status.slice(1)}
    </span>
  );
}
