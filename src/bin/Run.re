let config =
  Sihl.Core.Config.Setting.create(
    ~development=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_BACKEND", "console"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
    ],
    ~test=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_BACKEND", "memory"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
    ],
    ~production=[
      ("BASE_URL", "https://sihl-example-issues.oxidizing.io"),
      ("EMAIL_BACKEND", "sendgrid"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("SENDGRID_API_KEY", ""),
      ("DATABASE_URL", ""),
    ],
  );

let middlewares = [
  Sihl.Middleware.cookie,
  Sihl.Middleware.static,
  Sihl.Middleware.flash,
  Sihl.Middleware.error,
  Sihl.Middleware.db,
  Sihl_user.Middleware.Authn.token,
  Sihl_user.Middleware.Authn.session,
];

let bindings = [
  Sihl.Core.Registry.bind(
    Sihl.Core.Contract.Migration.repository,
    (module Sihl_repo_postgresql.Repo),
  ),
  Sihl.Core.Registry.bind(
    Sihl_email.Bind.Repository.key,
    (module Sihl_email_repo_postgresql.Repo),
  ),
  Sihl.Core.Registry.bind(
    Sihl_user.Binding.Repository.key,
    (module Sihl_user_repo_postgresql.Repo),
  ),
];

let project =
  Sihl.Run.Project.Project.create(
    ~config,
    ~bindings,
    middlewares,
    [
      (module Sihl_email.App),
      (module Sihl_user.App),
      (module Sihl_example_issues.App),
    ],
  );

let () = Sihl.Run.Project.Project.run_command(project);
