
import { Header } from "@/components/Header";
import { PageTransition } from "@/components/PageTransition";
import { useAuth } from "@/context/AuthContext";
import { Sidebar } from "@/components/Sidebar";

const About = () => {
  const { isAuthenticated, userRole } = useAuth();
  
  // Convert userRole to the format expected by Sidebar component
  const sidebarRole = userRole === "student" ? "user" : (userRole || "user");
  
  return (
    <div className="min-h-screen bg-background">
      <Header />
      {isAuthenticated && <Sidebar role={sidebarRole} />}
      
      <PageTransition>
        <main className={`pt-24 pb-16 px-8 animate-fade-in ${isAuthenticated ? 'pl-72' : ''}`}>
          <div className="max-w-6xl mx-auto bg-blue-50/80 p-8 rounded-lg shadow-sm">
            <h1 className="text-3xl font-semibold mb-6">About CC Portal</h1>
            
            <div className="space-y-6 text-muted-foreground">
              <p>
                Welcome to the Command Center Portal, a centralized platform designed to manage and monitor all your server resources efficiently.
              </p>
              
              <section>
                <h2 className="text-2xl font-medium mb-4 text-foreground">Our Mission</h2>
                <p>
                  Our mission is to provide a robust, secure, and user-friendly platform that simplifies the complexity of server management, enabling organizations to focus on their core business operations while maintaining complete visibility and control over their IT infrastructure.
                </p>
              </section>
              
              <section>
                <h2 className="text-2xl font-medium mb-4 text-foreground">What We Offer</h2>
                <p>
                  The CC Portal offers comprehensive server management tools, real-time monitoring, issue tracking, and request handling capabilities. Our platform is designed to cater to both administrators and regular users, providing differentiated access and functionality based on roles.
                </p>
              </section>
              
              <section>
                <h2 className="text-2xl font-medium mb-4 text-foreground">Our Team</h2>
                <p>
                  Our team consists of experienced IT professionals dedicated to delivering the best server management experience. We continuously improve our platform based on user feedback and industry best practices to ensure we meet the evolving needs of our users.
                </p>
              </section>
              
              <section>
                <h2 className="text-2xl font-medium mb-4 text-foreground">Contact Us</h2>
                <p>
                  For any inquiries or support, please reach out to our team at support@ccportal.example.com. We are available 24/7 to assist you with any questions or concerns.
                </p>
              </section>
            </div>
          </div>
        </main>
      </PageTransition>
    </div>
  );
};

export default About;
