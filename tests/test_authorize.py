import handlers.sessions_management_pb2 as sessions_management_protos
import handlers.authorization_pb2 as authorization_protos


# Start via `make test-debug` or `make test-release`
async def test_authorize_valid_session(grpc_authorization_service,
                                       mock_sessions_management):
    @mock_sessions_management('CheckSession')
    async def mock_check_session(request, context):
        assert request.session_id
        return sessions_management_protos.CheckSessionResponse(
            status=True,
        )

    request = authorization_protos.AuthorizeRequest(
        session_id='018e422da09a721686716a384edc359f')
    response = await grpc_authorization_service.Authorize(request)
    assert response.status


async def test_authorize_invalid_session(grpc_authorization_service,
                                         mock_sessions_management):
    @mock_sessions_management('CheckSession')
    async def mock_check_session(request, context):
        assert request.session_id
        return sessions_management_protos.CheckSessionResponse(
            status=False,
        )

    request = authorization_protos.AuthorizeRequest(
        session_id='018e422da09a721686716a384edc359f')
    response = await grpc_authorization_service.Authorize(request)
    assert not response.status
