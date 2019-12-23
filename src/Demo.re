open Express;
open Belt.Option;
open RouteAlertBehavior;

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";

let app = express();
let router = router();

let networkBridge = (request: serverRequest, respond) => {
  Js.Promise.(  
    Axios.get(request.path)
    |> then_(response => resolve(respond(response##data)))
  ) |> ignore;
};

App.use(app, Middleware.json());

App.options(app, ~path="/route_alerts") @@
Middleware.from((_, _, res) => {
  Response.setHeader("Access-Control-Allow-Origin", "*", res);
  Response.setHeader("Access-Control-Allow-Headers", "*", res);
  Response.sendString("", res);
});


Belt.Map.valuesToArray(endpointRegistry)->Belt.Array.map(endpoint => {
  App.post(app, ~path=endpoint.path) @@
  PromiseMiddleware.from((_, req, res) => {
    Js.log(endpoint.path)
    Response.setHeader("Access-Control-Allow-Origin", "*", res);
    Js.Promise.(
      make((~resolve, ~reject as _) => {
        Request.bodyJSON(req)
        ->getExn
        ->createRouteAlertEffectHandler(networkBridge, json => {
          [@bs] resolve(json);
        }) |> ignore
      })
      |> then_(response => {
        Js.log(response)
        resolve(Response.sendJson(response, res))
      })
    );
  }); 
});

App.options(app, ~path="/promise") @@
Middleware.from((_, _, res) => {
  Response.setHeader("Access-Control-Allow-Origin", "*", res);
  Response.setHeader("Access-Control-Allow-Headers", "*", res);
  Response.sendString("", res);
});

App.get(app, ~path="/promise") @@
 PromiseMiddleware.from((_, _, res) => {
    Response.setHeader("Access-Control-Allow-Origin", "*", res);  
    Js.Promise.(
      make((~resolve, ~reject as _) => setTimeout(() => [@bs] resolve("test string"), 1_000) |> ignore)
      |> then_(s => resolve(Response.sendString(s, res)))      
    )
  });

App.get(app, ~path="/") @@
Middleware.from((_req, _next) => Response.sendString("Hello, World!"));

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "Listening at http://127.0.0.1:3000"
  };

let server = App.listen(app, ~port=3000, ~onListen, ());
