open Types;

let text = ReasonReact.string;

// TODO: date-fns?
let formatDateTime = (date: Js.Date.t) => "2019-01-01";

[@react.component]
let make = (~streakName: string, ~streak: streak) =>
  <Paper>
    <Typography>
      {text(streakName ++ ": ")}
      <span className="marked">
        {text(string_of_int(streak.numberOfMatches) ++ "match(es)")}
      </span>
    </Typography>
    <Typography>
      {text("Started ")}
      <span className="date-time">
        {text(formatDateTime(streak.startedAt))}
      </span>
    </Typography>
    {streak.endedAt
     ->Belt.Option.mapWithDefault(ReasonReact.null, date =>
         <Typography>
           {text("Ended ")}
           <span className="date-time"> {text(formatDateTime(date))} </span>
           {streak.endedBy
            ->Belt.Option.mapWithDefault(ReasonReact.null, player =>
                <>
                  {text(" by ")}
                  <span className="marked"> {text(player.name)} </span>
                </>
              )}
         </Typography>
       )}
  </Paper>;
