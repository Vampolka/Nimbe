using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(NimbeSMSServer.Startup))]
namespace NimbeSMSServer
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
