// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Axios = require("axios");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Express = require("bs-express/src/Express.js");
var Process = require("process");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_js_exceptions = require("bs-platform/lib/js/caml_js_exceptions.js");
var RouteAlertBehavior = require("route-alert-behavior/src/RouteAlertBehavior.bs.js");

var app = Express.express(/* () */0);

var router = Express.router(undefined, undefined, undefined, /* () */0);

function networkBridge(request, respond) {
  Axios.get(request.path).then((function (response) {
          return Promise.resolve(Curry._1(respond, response.data));
        }));
  return /* () */0;
}

Express.App.use(app, Express.Middleware.json(undefined, undefined, undefined, /* () */0));

Express.App.options(app, "/route_alerts", Express.Middleware.from((function (param, param$1, res) {
            Express.$$Response.setHeader("Access-Control-Allow-Origin", "*", res);
            Express.$$Response.setHeader("Access-Control-Allow-Headers", "*", res);
            return Express.$$Response.sendString("", res);
          })));

Belt_Array.map(Belt_Map.valuesToArray(RouteAlertBehavior.endpointRegistry), (function (endpoint) {
        return Express.App.post(app, endpoint.path, Express.PromiseMiddleware.from((function (param, req, res) {
                          console.log(endpoint.path);
                          Express.$$Response.setHeader("Access-Control-Allow-Origin", "*", res);
                          return new Promise((function (resolve, param) {
                                          RouteAlertBehavior.createRouteAlertEffectHandler(Belt_Option.getExn(Express.$$Request.bodyJSON(req)), networkBridge, (function (json) {
                                                  return resolve(json);
                                                }));
                                          return /* () */0;
                                        })).then((function (response) {
                                        console.log(response);
                                        return Promise.resolve(Express.$$Response.sendJson(response, res));
                                      }));
                        })));
      }));

Express.App.options(app, "/promise", Express.Middleware.from((function (param, param$1, res) {
            Express.$$Response.setHeader("Access-Control-Allow-Origin", "*", res);
            Express.$$Response.setHeader("Access-Control-Allow-Headers", "*", res);
            return Express.$$Response.sendString("", res);
          })));

Express.App.get(app, "/promise", Express.PromiseMiddleware.from((function (param, param$1, res) {
            Express.$$Response.setHeader("Access-Control-Allow-Origin", "*", res);
            return new Promise((function (resolve, param) {
                            setTimeout((function (param) {
                                    return resolve("test string");
                                  }), 1000);
                            return /* () */0;
                          })).then((function (s) {
                          return Promise.resolve(Express.$$Response.sendString(s, res));
                        }));
          })));

Express.App.get(app, "/", Express.Middleware.from((function (_req, _next) {
            return (function (param) {
                return Express.$$Response.sendString("Hello, World!", param);
              });
          })));

function onListen(e) {
  var val;
  try {
    val = e;
  }
  catch (raw_exn){
    var exn = Caml_js_exceptions.internalToOCamlException(raw_exn);
    if (exn[0] === Js_exn.$$Error) {
      console.log(exn[1]);
      Process.exit(1);
      return /* () */0;
    } else {
      throw exn;
    }
  }
  console.log("Listening at http://127.0.0.1:3000");
  return /* () */0;
}

var server = Express.App.listen(app, 3000, onListen, /* () */0);

exports.app = app;
exports.router = router;
exports.networkBridge = networkBridge;
exports.onListen = onListen;
exports.server = server;
/* app Not a pure module */
