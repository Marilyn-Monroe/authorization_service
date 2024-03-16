import pathlib
import sys

import pytest

import handlers.sessions_management_pb2_grpc as sessions_management_services
import handlers.authorization_pb2_grpc as authorization_services

USERVER_CONFIG_HOOKS = ['prepare_service_config']

pytest_plugins = ['pytest_userver.plugins.grpc']


@pytest.fixture
def grpc_sessions_management_service(grpc_channel):
    return sessions_management_services.SessionsManagementServiceStub(
        grpc_channel)


@pytest.fixture
def grpc_authorization_service(grpc_channel):
    return authorization_services.AuthorizationServiceStub(
        grpc_channel)


@pytest.fixture(scope='session')
def mock_grpc_sessions_management_session(grpc_mockserver, create_grpc_mock):
    mock = create_grpc_mock(
        sessions_management_services.SessionsManagementServiceServicer)
    (sessions_management_services
        .add_SessionsManagementServiceServicer_to_server(
            mock.servicer, grpc_mockserver, ))
    return mock


@pytest.fixture(scope='session')
def mock_grpc_authorization_session(grpc_mockserver, create_grpc_mock):
    mock = create_grpc_mock(
        authorization_services.AuthorizationServiceServicer)
    (authorization_services
        .add_AuthorizationServiceServicer_to_server(
            mock.servicer, grpc_mockserver, ))
    return mock


@pytest.fixture
def mock_sessions_management(mock_grpc_sessions_management_session):
    with mock_grpc_sessions_management_session.mock() as mock:
        yield mock


@pytest.fixture
def mock_authorization(mock_grpc_authorization_session):
    with mock_grpc_authorization_session.mock() as mock:
        yield mock


@pytest.fixture(scope='session')
def prepare_service_config(grpc_mockserver_endpoint):
    def patch_config(config, config_vars):
        components = config['components_manager']['components']
        components['sessions-management-client'][
            'endpoint'] = grpc_mockserver_endpoint
        components['authorization-client'][
            'endpoint'] = grpc_mockserver_endpoint

    return patch_config


def pytest_configure(config):
    sys.path.append(str(
        pathlib.Path(__file__).parent.parent / 'proto/handlers/'))
