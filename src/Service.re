module Async = Sihl.Core.Async;

module Board = {
  let getAll = ((conn, user)) => {
    open! Sihl.Core.Http.Endpoint;
    if (!Sihl.Users.User.isAdmin(user)) {
      abort @@ Forbidden("Not allowed");
    };
    Repository.Board.GetAll.query(conn);
  };

  let getAllByUser = ((conn, user), ~userId) => {
    open! Sihl.Core.Http.Endpoint;
    if (!Sihl.Users.User.isAdmin(user) && user.id !== userId) {
      abort @@ Forbidden("Not allowed");
    };
    Repository.Board.GetAllByUser.query(conn, ~userId);
  };

  let create = ((conn, user), ~title) => {
    let board = Model.Board.make(~title, ~owner=Sihl.Users.User.id(user));
    let%Async _ = Repository.Board.Upsert.query(conn, ~board);
    Async.async(board);
  };
};

module Issue = {
  let getAll = ((conn, user)) => {
    open! Sihl.Core.Http.Endpoint;
    if (!Sihl.Users.User.isAdmin(user)) {
      abort @@ Forbidden("Not allowed");
    };
    Repository.Issue.GetAll.query(conn);
  };

  let getAllByBoard = ((conn, user), ~boardId) => {
    open! Sihl.Core.Http.Endpoint;
    let%Async board =
      Repository.Board.Get.query(conn, ~boardId)
      |> abortIfErr(NotFound("Board not found with that id"));
    if (!Sihl.Users.User.isAdmin(user) && user.id !== board.owner) {
      abort @@ Forbidden("Not allowed");
    };
    Repository.Issue.GetAllByBoard.query(conn, ~boardId);
  };

  let create = ((conn, user), ~title, ~description, ~board) => {
    open! Sihl.Core.Http.Endpoint;
    let%Async board =
      Repository.Board.Get.query(conn, ~boardId=board)
      |> abortIfErr(NotFound("Board not found with that id"));
    if (!Sihl.Users.User.isAdmin(user) && board.owner !== user.id) {
      abort @@ Forbidden("Not allowed");
    };
    let issue = Model.Issue.make(~title, ~description, ~board=board.id);
    let%Async _ = Repository.Issue.Upsert.query(conn, ~issue);
    Async.async(issue);
  };

  let complete = ((conn, user), ~issueId) => {
    open! Sihl.Core.Http.Endpoint;
    let%Async issue =
      Repository.Issue.Get.query(conn, ~issueId)
      |> abortIfErr(NotFound("Issue not found with that id"));
    let%Async board =
      Repository.Board.Get.query(conn, ~boardId=issue.board)
      |> abortIfErr(NotFound("Board not found with that id"));
    if (!Sihl.Users.User.isAdmin(user) && board.owner !== user.id) {
      abort @@ Forbidden("Not allowed");
    };
    let issue = Model.Issue.complete(issue);
    Repository.Issue.Upsert.query(conn, ~issue);
  };
};
