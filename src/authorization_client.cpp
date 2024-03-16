#include "authorization_client.hpp"

#include "userver/yaml_config/merge_schemas.hpp"

namespace authorization_service {

AuthorizationClient::AuthorizationClient(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : userver::components::LoggableComponentBase(config, component_context),
      client_factory_(
          component_context
              .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
              .GetFactory()),
      // The client needs a fixed endpoint
      client_(
          client_factory_.MakeClient<handlers::api::AuthorizationServiceClient>(
              "authorization-client", config["endpoint"].As<std::string>())) {}

bool AuthorizationClient::Authorize(std::string session_id) {
  handlers::api::AuthorizeRequest request;
  request.set_session_id(std::move(session_id));

  // Deadline must be set manually for each RPC
  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(
      userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

  // Initiate the RPC. No actual actions have been taken thus far besides
  // preparing to send the request.
  auto stream = client_.Authorize(request, std::move(context));

  // Complete the unary RPC by sending the request and receiving the response.
  // The client should call `Finish` (in case of single response) or `Read`
  // until `false` (in case of response stream), otherwise the RPC will be
  // cancelled.
  handlers::api::AuthorizeResponse response = stream.Finish();

  return response.status();
}

userver::yaml_config::Schema AuthorizationClient::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined wrapper around api::GreeterServiceClient that provides
    a simplified interface.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            Some other service endpoint (URI).
)");
}

void AppendAuthorizationClient(
    userver::components::ComponentList& component_list) {
  component_list.Append<AuthorizationClient>();
}

}  // namespace authorization_service