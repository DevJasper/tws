
import http;

using Config = http::Config;
using Server = http::Server;
using Router = http::Router;
using Middleware = http::Middleware;
using Handler = using RateLimiterMiddleware = middleware::RateLimiter;

void authMiddleware(auto request, auto response, auto next)
{
}

void homeHandler(auto request, auto response, auto next)
{
}

void loginHandler(Request& request)
{
    // Response response("Welcome", { .status = 200, .statusText = "OK", .headers = {
    //                                                                       { "Content-Type", "text/html" },
    //                                                                   } });
}

class HomeHandler : public Handler {

    auto Delete(Request) -> Response
    {
    }
}

class AuthMiddleware : public Middleware {
    void handle() override
    {
    }
}

int
main()
{
    Configuration config("config.json");

    Server server(config);
    server.addMiddleWare(RateLimitterMiddleware);

    Router router;
    // router.get("/", { authMiddleware }, [](Request& request) -> Response { });
    // router.get("/login", loginHandler);

    router.get("/", { AuthMiddleware }, HomeHandler);

    server.addRouter(router);

    server.start();

    return 0;
}
