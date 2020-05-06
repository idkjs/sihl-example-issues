module Layout = ClientLayout;

let login = (setError, ~email, ~password) => {
  let email = email->Belt.Option.getWithDefault("");
  let password = password->Belt.Option.getWithDefault("");
  let%Async result = ClientApi.User.Login.f(~email, ~password);
  Async.async(
    switch (result) {
    | Belt.Result.Ok({token, userId}) =>
      ClientSession.store(~token, ~userId);
      ReasonReactRouter.push("/app/boards/");
    | Belt.Result.Error(msg) => setError(_ => Some(msg))
    },
  );
};

[@react.component]
let make = () => {
  let (email, setEmail) = React.useState(() => None);
  let (password, setPassword) = React.useState(() => None);
  let (isLoading, setIsLoading) = React.useState(() => false);
  let canSubmit =
    switch (email, password) {
    | (Some(_), Some(_)) => true
    | _ => false
    };
  let (_, setError) = React.useContext(ClientContextProvider.Error.context);
  let login = login(setError);

  <Layout>
    <div className="columns">
      <div className="column is-one-quarter" />
      <div className="column is-two-quarters">
        <h2 className="title is-2"> {React.string("Login")} </h2>
        <div className="field">
          <label className="label"> {React.string("Email address")} </label>
          <div className="control has-icons-left">
            <input
              onChange={event => {
                let email = ClientUtils.wrapFormValue(event);
                setEmail(_ => email);
              }}
              value={email->Belt.Option.getWithDefault("")}
              className="input"
              name="email"
              type_="email"
              placeholder=""
            />
            <span className="icon is-small is-left">
              <i className="fas fa-envelope" />
            </span>
          </div>
        </div>
        <div className="field">
          <label className="label"> {React.string("Password")} </label>
          <div className="control has-icons-left">
            <input
              onChange={event => {
                let password = ClientUtils.wrapFormValue(event);
                setPassword(_ => password);
              }}
              value={password->Belt.Option.getWithDefault("")}
              onKeyDown={event =>
                ReactEvent.Keyboard.which(event) === 13
                  ? {
                    setIsLoading(_ => true);
                    login(~email, ~password)
                    ->Async.mapAsync(_ => setIsLoading(_ => false))
                    ->ignore;
                  }
                  : ()
              }
              className="input"
              name="password"
              type_="password"
              placeholder=""
            />
            <span className="icon is-small is-left">
              <i className="fas fa-lock" />
            </span>
          </div>
        </div>
        <div className="field is-grouped">
          <div className="control">
            <button
              className={"button is-link " ++ (isLoading ? "is-loading" : "")}
              disabled={!canSubmit}
              onClick={_ => {
                setIsLoading(_ => true);
                login(~email, ~password)
                ->Async.mapAsync(_ => setIsLoading(_ => false))
                ->ignore;
              }}>
              {React.string("Login")}
            </button>
          </div>
        </div>
        <a
          href="/app/password-reset"
          onClick={event => {
            ReactEvent.Mouse.preventDefault(event);
            ReasonReactRouter.push("/app/password-reset");
          }}>
          {React.string("Forgot your password?")}
        </a>
      </div>
      <div className="column is-one-quarter" />
    </div>
  </Layout>;
};
