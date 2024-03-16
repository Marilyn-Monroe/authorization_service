#pragma once

#include <handlers/authorization_client.usrv.pb.hpp>
#include "userver/components/component.hpp"
#include "userver/components/component_list.hpp"
#include "userver/components/loggable_component_base.hpp"
#include "userver/ugrpc/client/client_factory_component.hpp"

namespace authorization_service {

class AuthorizationClient final
    : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "authorization-client";

  AuthorizationClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context);

  bool Authorize(std::string session_id);

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  handlers::api::AuthorizationServiceClient client_;
};

void AppendAuthorizationClient(
    userver::components::ComponentList& component_list);

}  // namespace authorization_service

template <>
inline constexpr bool userver::components::kHasValidate<
    authorization_service::AuthorizationClient> = true;