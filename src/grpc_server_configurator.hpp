#pragma once

#include <userver/components/component_list.hpp>
#include <userver/components/loggable_component_base.hpp>

namespace authorization_service {

namespace {

class GrpcServerConfigurator final
    : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "grpc-server-configurator";

  GrpcServerConfigurator(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context);

  ~GrpcServerConfigurator() override;
};

}  // namespace

void AppendGrpcServerConfigurator(
    userver::components::ComponentList& component_list);

}  // namespace authorization_service