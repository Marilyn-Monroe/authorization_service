#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/utils/daemon_run.hpp>

#include "authorization_client.hpp"
#include "authorization_service.hpp"
#include "grpc_server_configurator.hpp"
#include "hello.hpp"
#include "sessions_management_client.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::clients::dns::Component>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<userver::ugrpc::server::ServerComponent>()
          .Append<userver::ugrpc::client::ClientFactoryComponent>();

  authorization_service::AppendHello(component_list);
  authorization_service::AppendAuthorizationService(component_list);
  authorization_service::AppendSessionsManagementClient(component_list);
  authorization_service::AppendAuthorizationClient(component_list);
  authorization_service::AppendGrpcServerConfigurator(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
