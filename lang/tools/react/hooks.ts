// Detoprox — React Hooks mínimos (TS)
// Independientes del cliente: requieren un objeto con método request(method, path, body?)

import * as React from "react";

// Cliente mínimo compatible con el generado por ts_client_gen
export type DPXClient = {
  request<T = any>(method: string, path: string, body?: any): Promise<T>;
};

// Utilidad para componer querystring si quieres pasar params externos
export function withQuery(path: string, query?: Record<string, string | number | boolean>) {
  if (!query || Object.keys(query).length === 0) return path;
  const qs = Object.entries(query)
    .map(([k, v]) => `${encodeURIComponent(k)}=${encodeURIComponent(String(v))}`)
    .join("&");
  return path.includes("?") ? `${path}&${qs}` : `${path}?${qs}`;
}

// GET: carga + refetch
export function useDetoproxQuery<T = any>(
  client: DPXClient,
  path: string,
  deps: React.DependencyList = []
) {
  const [data, setData] = React.useState<T | null>(null);
  const [error, setError] = React.useState<any>(null);
  const [loading, setLoading] = React.useState<boolean>(true);

  const fetcher = React.useCallback(async () => {
    setLoading(true);
    setError(null);
    try {
      const res = await client.request<T>("GET", path);
      setData(res);
    } catch (e) {
      setError(e);
    } finally {
      setLoading(false);
    }
  }, [client, path]);

  React.useEffect(() => {
    let alive = true;
    (async () => {
      try {
        setLoading(true);
        const res = await client.request<T>("GET", path);
        if (!alive) return;
        setData(res);
      } catch (e) {
        if (!alive) return;
        setError(e);
      } finally {
        if (!alive) return;
        setLoading(false);
      }
    })();
    return () => {
      alive = false;
    };
  }, [client, path, ...deps]);

  return { data, error, loading, refetch: fetcher };
}

// POST/PUT/DELETE: mutate + estados
export function useDetoproxMutation<T = any, B = any>(
  client: DPXClient,
  method: "POST" | "PUT" | "DELETE",
  path: string
) {
  const [state, setState] = React.useState<"idle" | "loading" | "success" | "error">("idle");
  const [error, setError] = React.useState<any>(null);

  const mutate = React.useCallback(
    async (body?: B) => {
      setState("loading");
      setError(null);
      try {
        const res = await client.request<T>(method, path, body);
        setState("success");
        return res;
      } catch (e) {
        setState("error");
        setError(e);
        throw e;
      }
    },
    [client, method, path]
  );

  return { mutate, state, error };
}
