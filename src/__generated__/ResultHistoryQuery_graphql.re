/* @generated */

module Types = {
  [@ocaml.warning "-30"];
  type response_results_connection = {
    fragmentRefs:
      ReasonRelay.fragmentRefs([ | `ResultsTable_Results | `Stats_Results]),
  }
  and response_community_settings_connection = {
    edges: array(response_community_settings_connection_edges),
  }
  and response_community_settings_connection_edges = {
    node: response_community_settings_connection_edges_node,
  }
  and response_community_settings_connection_edges_node = {
    fragmentRefs:
      ReasonRelay.fragmentRefs(
        [
          | `ResultCommunitySettings_IncludeExtraTime
          | `ResultsTableHeader_CommunitySettings
          | `StatsTableHeader_ScoreType
        ],
      ),
  };

  type response = {
    results_connection: response_results_connection,
    community_settings_connection: response_community_settings_connection,
  };
  type rawResponse = response;
  type refetchVariables = {
    communityName: option(string),
    dateFrom: option(DateTimeUtils.Datetime.t),
    dateTo: option(DateTimeUtils.Datetime.t),
  };
  let makeRefetchVariables =
      (~communityName=?, ~dateFrom=?, ~dateTo=?, ()): refetchVariables => {
    communityName,
    dateFrom,
    dateTo,
  };
  type variables = {
    communityName: string,
    dateFrom: option(DateTimeUtils.Datetime.t),
    dateTo: option(DateTimeUtils.Datetime.t),
  };
};

module Internal = {
  type responseRaw;
  let responseConverter: Js.Dict.t(Js.Dict.t(Js.Dict.t(string))) = [%raw
    {json| {"__root":{"results_connection":{"f":""},"community_settings_connection_edges_node":{"f":""}}} |json}
  ];
  let responseConverterMap = ();
  let convertResponse = v =>
    v
    ->ReasonRelay._convertObj(
        responseConverter,
        responseConverterMap,
        Js.undefined,
      );

  type rawResponseRaw = responseRaw;
  let convertRawResponse = convertResponse;

  let variablesConverter: Js.Dict.t(Js.Dict.t(Js.Dict.t(string))) = [%raw
    {json| {"__root":{"dateFrom":{"n":"","c":"DateTimeUtils.Datetime"},"dateTo":{"n":"","c":"DateTimeUtils.Datetime"}}} |json}
  ];
  let variablesConverterMap = {
    "DateTimeUtils.Datetime": DateTimeUtils.Datetime.serialize,
  };
  let convertVariables = v =>
    v
    ->ReasonRelay._convertObj(
        variablesConverter,
        variablesConverterMap,
        Js.undefined,
      );
};

type queryRef;

module Utils = {
  open Types;
  let makeVariables = (~communityName, ~dateFrom=?, ~dateTo=?, ()): variables => {
    communityName,
    dateFrom,
    dateTo,
  };
};

type operationType = ReasonRelay.queryNode;

let node: operationType = [%raw
  {json| (function(){
var v0 = [
  {
    "defaultValue": null,
    "kind": "LocalArgument",
    "name": "communityName"
  },
  {
    "defaultValue": null,
    "kind": "LocalArgument",
    "name": "dateFrom"
  },
  {
    "defaultValue": null,
    "kind": "LocalArgument",
    "name": "dateTo"
  }
],
v1 = {
  "fields": [
    {
      "fields": [
        {
          "kind": "Variable",
          "name": "_eq",
          "variableName": "communityName"
        }
      ],
      "kind": "ObjectValue",
      "name": "name"
    }
  ],
  "kind": "ObjectValue",
  "name": "community"
},
v2 = [
  {
    "kind": "Literal",
    "name": "order_by",
    "value": {
      "date": "desc"
    }
  },
  {
    "fields": [
      (v1/*: any*/),
      {
        "fields": [
          {
            "kind": "Variable",
            "name": "_gte",
            "variableName": "dateFrom"
          },
          {
            "kind": "Variable",
            "name": "_lte",
            "variableName": "dateTo"
          }
        ],
        "kind": "ObjectValue",
        "name": "date"
      }
    ],
    "kind": "ObjectValue",
    "name": "where"
  }
],
v3 = [
  {
    "fields": [
      (v1/*: any*/)
    ],
    "kind": "ObjectValue",
    "name": "where"
  }
],
v4 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "id",
  "storageKey": null
},
v5 = [
  (v4/*: any*/),
  {
    "alias": null,
    "args": null,
    "kind": "ScalarField",
    "name": "name",
    "storageKey": null
  }
];
return {
  "fragment": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Fragment",
    "metadata": null,
    "name": "ResultHistoryQuery",
    "selections": [
      {
        "alias": null,
        "args": (v2/*: any*/),
        "concreteType": "resultsConnection",
        "kind": "LinkedField",
        "name": "results_connection",
        "plural": false,
        "selections": [
          {
            "args": null,
            "kind": "FragmentSpread",
            "name": "ResultsTable_Results"
          },
          {
            "args": null,
            "kind": "FragmentSpread",
            "name": "Stats_Results"
          }
        ],
        "storageKey": null
      },
      {
        "alias": null,
        "args": (v3/*: any*/),
        "concreteType": "community_settingsConnection",
        "kind": "LinkedField",
        "name": "community_settings_connection",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": null,
            "concreteType": "community_settingsEdge",
            "kind": "LinkedField",
            "name": "edges",
            "plural": true,
            "selections": [
              {
                "alias": null,
                "args": null,
                "concreteType": "community_settings",
                "kind": "LinkedField",
                "name": "node",
                "plural": false,
                "selections": [
                  {
                    "args": null,
                    "kind": "FragmentSpread",
                    "name": "ResultCommunitySettings_IncludeExtraTime"
                  },
                  {
                    "args": null,
                    "kind": "FragmentSpread",
                    "name": "ResultsTableHeader_CommunitySettings"
                  },
                  {
                    "args": null,
                    "kind": "FragmentSpread",
                    "name": "StatsTableHeader_ScoreType"
                  }
                ],
                "storageKey": null
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ],
    "type": "query_root",
    "abstractKey": null
  },
  "kind": "Request",
  "operation": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Operation",
    "name": "ResultHistoryQuery",
    "selections": [
      {
        "alias": null,
        "args": (v2/*: any*/),
        "concreteType": "resultsConnection",
        "kind": "LinkedField",
        "name": "results_connection",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": null,
            "concreteType": "resultsEdge",
            "kind": "LinkedField",
            "name": "edges",
            "plural": true,
            "selections": [
              {
                "alias": null,
                "args": null,
                "concreteType": "results",
                "kind": "LinkedField",
                "name": "node",
                "plural": false,
                "selections": [
                  {
                    "alias": null,
                    "args": null,
                    "concreteType": "players",
                    "kind": "LinkedField",
                    "name": "player1",
                    "plural": false,
                    "selections": (v5/*: any*/),
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "concreteType": "players",
                    "kind": "LinkedField",
                    "name": "player2",
                    "plural": false,
                    "selections": (v5/*: any*/),
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "player2goals",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "player1goals",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "extratime",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "date",
                    "storageKey": null
                  },
                  (v4/*: any*/)
                ],
                "storageKey": null
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      },
      {
        "alias": null,
        "args": (v3/*: any*/),
        "concreteType": "community_settingsConnection",
        "kind": "LinkedField",
        "name": "community_settings_connection",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": null,
            "concreteType": "community_settingsEdge",
            "kind": "LinkedField",
            "name": "edges",
            "plural": true,
            "selections": [
              {
                "alias": null,
                "args": null,
                "concreteType": "community_settings",
                "kind": "LinkedField",
                "name": "node",
                "plural": false,
                "selections": [
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "include_extra_time",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "score_type",
                    "storageKey": null
                  },
                  (v4/*: any*/)
                ],
                "storageKey": null
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ]
  },
  "params": {
    "cacheID": "b2155c2e4b05dfd5b5fdab7af9918cf4",
    "id": null,
    "metadata": {},
    "name": "ResultHistoryQuery",
    "operationKind": "query",
    "text": "query ResultHistoryQuery(\n  $communityName: String!\n  $dateFrom: timestamptz\n  $dateTo: timestamptz\n) {\n  results_connection(where: {community: {name: {_eq: $communityName}}, date: {_gte: $dateFrom, _lte: $dateTo}}, order_by: {date: desc}) {\n    ...ResultsTable_Results\n    ...Stats_Results\n  }\n  community_settings_connection(where: {community: {name: {_eq: $communityName}}}) {\n    edges {\n      node {\n        ...ResultCommunitySettings_IncludeExtraTime\n        ...ResultsTableHeader_CommunitySettings\n        ...StatsTableHeader_ScoreType\n        id\n      }\n    }\n  }\n}\n\nfragment ResultCommunitySettings_IncludeExtraTime on community_settings {\n  include_extra_time\n}\n\nfragment Result_SingleResult on results {\n  player1 {\n    id\n    name\n  }\n  player2 {\n    id\n    name\n  }\n  player2goals\n  player1goals\n  extratime\n  date\n  id\n}\n\nfragment ResultsTableHeader_CommunitySettings on community_settings {\n  score_type\n  include_extra_time\n}\n\nfragment ResultsTable_Results on resultsConnection {\n  edges {\n    node {\n      ...Result_SingleResult\n      player1 {\n        id\n        name\n      }\n      player2 {\n        id\n        name\n      }\n      player2goals\n      player1goals\n      extratime\n      date\n      id\n    }\n  }\n}\n\nfragment StatsTableHeader_ScoreType on community_settings {\n  score_type\n}\n\nfragment Stats_Results on resultsConnection {\n  edges {\n    node {\n      player1 {\n        id\n        name\n      }\n      player2 {\n        id\n        name\n      }\n      player2goals\n      player1goals\n      extratime\n      date\n      id\n    }\n  }\n}\n"
  }
};
})() |json}
];

include ReasonRelay.MakeLoadQuery({
  type variables = Types.variables;
  type loadedQueryRef = queryRef;
  type response = Types.response;
  let query = node;
  let convertVariables = Internal.convertVariables;
});
