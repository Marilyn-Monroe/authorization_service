#include "authorization_service.hpp"

#include "userver/components/component.hpp"

namespace authorization_service {

namespace {

AuthorizationService::AuthorizationService(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : handlers::api::AuthorizationServiceBase::Component(config,
                                                         component_context),
      authorization_client_(
          component_context.FindComponent<AuthorizationClient>()),
      sessions_management_client_(
          component_context.FindComponent<SessionsManagementClient>()) {}

void AuthorizationService::Authorize(
    handlers::api::AuthorizationServiceBase::AuthorizeCall& call,
    handlers::api::AuthorizeRequest&& request) {
  handlers::api::AuthorizeResponse response;

  const auto& session_id = request.session_id();

  try {
    auto status = sessions_management_client_.CheckSession(session_id);
    response.set_status(status);
    call.Finish(response);
  } catch (const userver::ugrpc::client::UnavailableError& e) {
    call.FinishWithError(grpc::Status(grpc::StatusCode::UNAVAILABLE, e.what()));
  }
}

}  // namespace

void AppendAuthorizationService(
    userver::components::ComponentList& component_list) {
  component_list.Append<AuthorizationService>();
}

}  // namespace authorization_service