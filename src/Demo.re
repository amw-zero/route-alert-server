open Express;
open Belt.List;
open Belt.Option;
open RouteAlertBehavior;

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";

let app = express();
let router = router();

/*
 let requests = [
   "/1",`
   "/2",
   "/3"
 ];

 forEach(requests, r => App.get(app, ~path=r) @@ Middleware.from((_req, _next) => Response.sendString("test")))
 */

let networkBridge = (request, respond) => {
  Js.Promise.(  
    Axios.get(request.path)
    |> then_(response => resolve(respond(response##data)))
  );
};

App.use(app, Middleware.json());

App.options(app, ~path="/route_alerts") @@
Middleware.from((_, _, res) => {
  Response.setHeader("Access-Control-Allow-Origin", "*", res);
  Response.setHeader("Access-Control-Allow-Headers", "*", res);
  Response.sendString("", res);
});

let mapApi = "https://maps.googleapis.com/maps/api/directions/json?origin=Port+Authority&destination=20+Remington+Dr+Freehold&key=AIzaSyC6AfIwElNGcfmzz-XyBHUb3ftWb2SL2vU&departure_time=now"

App.post(app, ~path="/route_alerts") @@
  PromiseMiddleware.from((next, req, res) => {
    Js.log("/route_alerts")
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


App.post(app, ~path="/route_alerts-axios") @@
  PromiseMiddleware.from((req, next, res) => {
    Js.Promise.(
      Axios.get(mapApi)
      |> then_(response => resolve(Response.sendJson(response##data, res)))
    )
  });

App.get(app, ~path="/promise") @@
 PromiseMiddleware.from((req, next, res) => {
    Js.Promise.(
      make((~resolve, ~reject as _) => setTimeout(() => [@bs] resolve("test string"), 5_000) |> ignore)
      |> then_(s => resolve(Response.sendString(s, res)))      
    )
  });

// PromiseMiddleware.from((req, next, res) => {
//   Js.Promise.(
//     make((~resolve, ~reject as _) => {
//       [@bs] resolve("s");
// )

//     |> then_(s => resolve(Response.sendString(s, res)));
//   );
//);

  // Response.setHeader("Access-Control-Allow-Origin", "*", res);
  // Js.log(Request.bodyJSON(req));
  // Request.bodyJSON(req)
  // ->getExn
  // ->createRouteAlertEffectHandler(networkBridge, json => {
  //   Js.Promise.resolve(Response.sendJson(json));
  // });
//  ->Response.sendJson(res);

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
