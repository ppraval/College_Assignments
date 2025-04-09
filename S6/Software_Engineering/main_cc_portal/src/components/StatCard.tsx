
import { cn } from "@/lib/utils";

interface StatCardProps {
  title: string;
  value: string | number;
  className?: string;
}

export function StatCard({ title, value, className }: StatCardProps) {
  return (
    <div className={cn(
      "bg-white rounded-lg shadow-sm p-6 border border-border transition-all duration-300 hover:shadow-md",
      className
    )}>
      <h3 className="text-sm font-medium text-muted-foreground mb-2">{title}</h3>
      <p className="text-2xl font-semibold">{value}</p>
    </div>
  );
}
