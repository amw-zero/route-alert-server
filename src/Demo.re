open Express;

let app = express();
let router = router();

let routes = [|
  "/1",
  "/2",
  "/3"
|];

for (i in 0 to 2) {
   App.get(app, ~path=routes[i]) @@ Middleware.from((_req, _next) => Response.sendString(string_of_int(i)))
 }

Router.post(router, ~path="/route_alerts")
@@ Middleware.from((_req, _next) => 
    Response.sendString("{msg:\"test\"}"));

App.get(app, ~path="/") @@ Middleware.from((_req, _next) => Response.sendString("Hello, World!"));
    
let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "Listening at http://127.0.0.1:3000"
  };

let server = App.listen(app, ~port=3000, ~onListen, ());
