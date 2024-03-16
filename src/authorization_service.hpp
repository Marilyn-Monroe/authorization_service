#pragma once

#include "authorization_client.hpp"
#include "sessions_management_client.hpp"

#include <handlers/authorization_service.usrv.pb.hpp>
#include "userver/components/component_list.hpp"

namespace authorization_service {

namespace {

class AuthorizationService final
    : public handlers::api::AuthorizationServiceBase::Component {
 public:
  static constexpr std::string_view kName = "authorization-service";

  AuthorizationService(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context);

  void Authorize(handlers::api::AuthorizationServiceBase::AuthorizeCall& call,
                 handlers::api::AuthorizeRequest&& request) final;

 private:
  AuthorizationClient& authorization_client_;
  SessionsManagementClient& sessions_management_client_;
};

}  // namespace

void AppendAuthorizationService(
    userver::components::ComponentList& component_list);

}  // namespace authorization_service