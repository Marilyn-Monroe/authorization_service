#include "grpc_server_configurator.hpp"

#include <userver/components/component.hpp>
#include <userver/ugrpc/server/server_component.hpp>

namespace authorization_service {

namespace {

GrpcServerConfigurator::GrpcServerConfigurator(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : LoggableComponentBase(config, context) {
  context.FindComponent<userver::ugrpc::server::ServerComponent>()
      .GetServer()
      .WithServerBuilder([](auto& builder) {
        auto server_address = "0.0.0.0:8081";
        builder.AddListeningPort(server_address,
                                 grpc::InsecureServerCredentials());
      });
}

GrpcServerConfigurator::~GrpcServerConfigurator() = default;

}  // namespace

void AppendGrpcServerConfigurator(
    userver::components::ComponentList& component_list) {
  component_list.Append<GrpcServerConfigurator>();
}

}  // namespace authorization_service