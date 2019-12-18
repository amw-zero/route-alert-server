open Express;
open Belt.List;
open Belt.Option;
open RouteAlertBehavior;

let app = express();
let router = router();

/*
let requests = [
  "/1",
  "/2",
  "/3"
];

forEach(requests, r => App.get(app, ~path=r) @@ Middleware.from((_req, _next) => Response.sendString("test")))
*/

App.use(app, Middleware.json());


App.post(app, ~path="/route_alerts")
@@ Middleware.from((_next, req) => {
  Js.log(Request.bodyJSON(req));
  Request.bodyJSON(req)
    ->getExn
    ->createRouteAlertEffectHandler
    ->Response.sendJson
});


App.get(app, ~path="/") @@ Middleware.from((_req, _next) => Response.sendString("Hello, World!"));
    
let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "Listening at http://127.0.0.1:3000"
  };

let server = App.listen(app, ~port=3000, ~onListen, ());
