open Tc;

module UnlockWallet = [%graphql
  {| mutation unlock($password: String, $publicKey: PublicKey) {
      unlockWallet(input: {password: $password, publicKey: $publicKey}) {
          publicKey
        }
  } |}
];

module UnlockMutation = ReasonApollo.CreateMutation(UnlockWallet);

type modalState = {error: option(string)};

[@react.component]
let make = (~wallet, ~onClose, ~onSuccess) => {
  let (error, setError) = React.useState(() => None);
  let (password, setPassword) = React.useState(() => "");
  let variables =
    UnlockWallet.make(
      ~password,
      ~publicKey=Apollo.Encoders.publicKey(wallet),
      (),
    )##variables;
  <Modal title="Unlock Wallet" onRequestClose=onClose>
    <div className=Modal.Styles.default>
      <p className=Theme.Text.Body.regular>
        {React.string("Please enter password for ")}
        <WalletName pubkey=wallet />
        {React.string(".")}
      </p>
      <Spacer height=1. />
      <TextField
        label="Pass"
        type_="password"
        onChange={value => setPassword(_ => value)}
        value=password
        ?error
      />
      <Spacer height=1.5 />
      <div className=Css.(style([display(`flex)]))>
        <Button label="Cancel" style=Button.Gray onClick={_ => onClose()} />
        <Spacer width=1. />
        <UnlockMutation>
          ...{(mutation, {result}) =>
            <>
              <Button
                label="Unlock"
                style=Button.Green
                onClick={_ =>
                  mutation(
                    ~variables,
                    ~refetchQueries=[|"getWallets", "walletLocked"|],
                    (),
                  )
                  |> ignore
                }
              />
              {switch (result) {
               | NotCalled => React.null
               | Data(_) =>
                 onSuccess();
                 React.null;
               | Error(err) =>
                 let message =
                   err##graphQLErrors
                   |> Js.Nullable.toOption
                   |> Option.withDefault(~default=Array.empty)
                   |> Array.get(~index=0)
                   |> Option.map(~f=e => e##message)
                   |> Option.withDefault(~default="Server error");
                 setError(_ => Some(message));
                 React.null;
               | Loading => React.null
               }}
            </>
          }
        </UnlockMutation>
      </div>
    </div>
  </Modal>;
};