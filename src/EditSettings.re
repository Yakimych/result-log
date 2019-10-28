open Utils;
open Queries;
open Types;

type editCommunitySettingsAction =
  | SetAllSettings(communitySettings)
  | ToggleAllowDraws
  | SetScoreType(scoreType)
  | SetMaxSelectablePoints(int);

let reducer = (settings, action): communitySettings =>
  switch (action) {
  | SetAllSettings(allSettings) => allSettings
  | ToggleAllowDraws => {...settings, allowDraws: !settings.allowDraws}
  | SetScoreType(scoreType) => {...settings, scoreType}
  | SetMaxSelectablePoints(maxSelectablePoints) => {
      ...settings,
      maxSelectablePoints,
    }
  };

let scoreTypes = [|`Goals, `Points|]->Belt.Array.map(scoreTypeToString);

[@react.component]
let make = (~communityName: string) => {
  let settingsQueryConfig =
    CommunitySettingsQueryConfig.make(~communityName, ());
  let (_, fullSettingsQuery) =
    CommunitySettingsQuery.use(~variables=settingsQueryConfig##variables, ());

  let (state, dispatch) =
    React.useReducer(reducer, defaultCommunitySettings);

  let saveCommunitySettings = () => {
    Js.log(state);
  };

  React.useEffect1(
    _ => {
      switch (fullSettingsQuery.data) {
      | Some(data) =>
        dispatch(SetAllSettings(data |> toCommunitySettings)) |> ignore
      | None => ()
      };
      None;
    },
    [|fullSettingsQuery.data|],
  );

  // TODO: Handle loading and error states
  switch (fullSettingsQuery.data) {
  | None => <div />
  | Some(_) =>
    <>
      <div>
        {text("Allow draws: ")}
        <input
          type_="checkbox"
          checked={state.allowDraws}
          onChange={_ => dispatch(ToggleAllowDraws)}
        />
      </div>
      <div>
        {text("Max selectable points:")}
        <input
          type_="number"
          value={string_of_int(state.maxSelectablePoints)}
          onChange={e => {
            let selectablePointsString = ReactEvent.Form.target(e)##value;
            dispatch(
              SetMaxSelectablePoints(int_of_string(selectablePointsString)),
            );
          }}
        />
      </div>
      <div>
        {text("Score type: ")}
        <select
          value={state.scoreType |> scoreTypeToString}
          onChange={e => {
            let scoreTypeString = ReactEvent.Form.target(e)##value;
            dispatch(SetScoreType(scoreTypeString |> toScoreType));
          }}>
          {scoreTypes->Belt.Array.map(scoreType =>
             <option value=scoreType key=scoreType>
               {text(scoreType)}
             </option>
           )
           |> React.array}
        </select>
      </div>
      <button onClick={_ => saveCommunitySettings()}> {text("Save")} </button>
    </>
  };
};